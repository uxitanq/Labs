#include "D:\labs\l7\second.h"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\first.h"

first::first(const std::string& name, int course, const std::string& group, const std::string& studentCardNumber, const std::array<int, 4>& grades)
    : Student(name, course, group, studentCardNumber), grades(grades) {}

first::first(const first& other)
    : Student(other), grades(other.grades) {}

first::~first (){
    
}

int first::getGrade(int index) const {
    return grades[index];
}

void first::setGrade(int index, int grade) {
    grades[index] = grade;
}

void first::printAverageGrade() const {
    int sum = 0;
    for (int grade : grades) 
        sum += grade;
    std::cout << "Average Grade after First Session: " << double(sum) / grades.size() << std::endl;
}

double first::getAverage() const {
    int sum = 0;
    for (int grade : grades) {
        sum += grade;
    }
    return double(sum) / grades.size();
}

std::ostream& operator<<(std::ostream& os, const first& student) {
    os << static_cast<const Student&>(student) << ", Grades: ";
    for (int grade : student.grades) {
        os << grade << " ";
    }
    return os;
}
