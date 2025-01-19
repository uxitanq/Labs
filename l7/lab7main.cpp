#include "D:\labs\l7\first.h"
#include "D:\labs\l7\first.cpp"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\student.cpp"
#include "D:\labs\l7\second.h"
#include "D:\labs\l7\second.cpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>

int main() {
    first student1("Alice", 1, "1", "123A", {5, 4, 8, 10});
    second student2("Bob", 2, "3", "456B", {7, 8, 8, 5}, {8, 8, 9, 8, 7});

    std::vector<Student*> students = {&student1, &student2};
    
    double av = 0;

    for (Student* student : students) {
        std::cout << *student << std::endl;
        student->printAverageGrade();
        av+= student->getAverage();
    }
    std::cout << std::fixed <<std::setprecision (2) << "Group average is " << av / 2 << std::endl;
    return 0;
}