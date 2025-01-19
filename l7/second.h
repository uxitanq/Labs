#ifndef second_h
#define second_h
#include "first.h"
#include <array>

class second : public first {
private:
    std::array<int, 5> secondTermGrades;

public:
    second(const std::string &name, int course, const std::string &group, const std::string &studentCardNumber,
                         const std::array<int, 4> &firstTermGrades, const std::array<int, 5> &secondTermGrades);
    second(const second& other);
    second() = delete;
   ~second();

    int getSecondTermGrade(int index) const;
    void setSecondTermGrade(int index, int grade);

    void printAverageGrade() const override;
    double getAverage() const override;

    friend std::ostream& operator<<(std::ostream& os, const second& student);
};

#endif 
