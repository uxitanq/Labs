#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <utility>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : capacity_(size > 0 ? size : 0), closed_(false) {}

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        cv_.wait(lock, [this]() { 
            return closed_ || buffer_.size() < capacity_; 
        });
        
        if (closed_) {
            throw std::runtime_error("Channel is closed");
        }
        
        buffer_.push(std::move(value));
        cv_.notify_all(); 
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        cv_.wait(lock, [this]() { 
            return closed_ || !buffer_.empty(); 
        });
        
        if (!buffer_.empty()) {
            T value = std::move(buffer_.front());
            buffer_.pop();
            cv_.notify_all(); 
            return {std::move(value), true};
        }
        
        return {T(), false};
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_ = true;
        cv_.notify_all();
    }

private:
    std::queue<T> buffer_;
    const size_t capacity_;
    bool closed_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif // BUFFERED_CHANNEL_H_