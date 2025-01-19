#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <algorithm>

class Stack {
private:
    std::vector<double> data;

public:
    Stack& operator<<(double value) {
        data.push_back(value);
        return *this;
    }

    Stack& operator>>(double& value) {
      if (data.empty()) 
            throw std::runtime_error("Stack is empty");
        value = data.back();
        data.pop_back();
        return *this;
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }
    
    bool operator<(const Stack& other) const {
        return data.size() < other.data.size();
    }

    bool operator==(const Stack& other) const {
        if (data.size() != other.data.size())
            return false;
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] != other.data[i])
                return false;
        }
        return true;
    }

    double operator[](size_t index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t size() const {
        return data.size();
    }
    
    bool isEmpty() const {
        return data.empty();
    }
    
    void clear(){
        data.clear();
    }
    
    /**void print() const{
        std::cout << "[ ";
        for(const auto& elem : data){
            std::cout << elem << " ";
        }
        std::cout << "]" << std::endl;
    }**/
    
};