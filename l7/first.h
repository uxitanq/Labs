
#ifndef first_h
#define first_h
#include "student.h"
#include <array>

class first : public Student {
private:
    std::array<int, 4> grades;

public:
    first(const std::string& name, int course, const std::string& group, const std::string& studentCardNumber, const std::array<int, 4>& grades);
    first(const first& other);
    first() = delete;
   ~first();

    int getGrade(int index) const;
    void setGrade(int index, int grade);

    void printAverageGrade() const override;
    double getAverage() const override;

    friend std::ostream& operator<<(std::ostream& os, const first& student);
};

#endif
