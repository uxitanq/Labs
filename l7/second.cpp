#include "D:\labs\l7\second.h"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\first.h"
#include <iomanip>

second::second(const std::string &name, int course, const std::string &group, const std::string &studentCardNumber,
                                           const std::array<int, 4> &firstTermGrades, const std::array<int, 5> &secondTermGrades)
    : first(name, course, group, studentCardNumber, firstTermGrades), secondTermGrades(secondTermGrades) {}

second::second(const second& other)
    : first(other), secondTermGrades(other.secondTermGrades) {}

second::~second (){
    
}

int second::getSecondTermGrade(int index) const {
    return secondTermGrades[index];
}

void second::setSecondTermGrade(int index, int grade) {
    secondTermGrades[index] = grade;
}

void second::printAverageGrade() const {
    int sum = 0;
    for (int grade : secondTermGrades) {
        sum += grade;
    }
    double summ = first::getAverage() * 4 + sum;
    std::cout << std::fixed <<std::setprecision (2) << "Average Grade for Year: " << summ / 9 << std::endl;
}

double second::getAverage() const {
    int sum = 0;
    for (int grade : secondTermGrades) {
        sum += grade;
    }
    double summ = first::getAverage() * 4 + sum;
    return summ / 9;
}

std::ostream& operator<<(std::ostream& os, const second& student) {
    os << static_cast<const first&>(student) << ", Second Term Grades: ";
    for (int grade : student.secondTermGrades) {
        os << grade << " ";
    }
    return os;
}
