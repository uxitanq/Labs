#include "D:\labs\l7\first.h"
#include "D:\labs\l7\first.cpp"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\student.cpp"
#include "D:\labs\l7\second.h"
#include "D:\labs\l7\second.cpp"
#include <iostream>
#include <vector>
#include <numeric>

int main() {
    int gr1[] = {4, 4, 4, 4};
    int gr2[] = {4, 4, 4, 4, 4};
    int gr3[] = {5, 5, 5, 5};
    int gr4[] = {5, 5, 5, 5, 5};
    int gr5[] = {3, 3, 3, 3};
    int gr6[] = {3, 3, 3, 3, 3};
    second student1("Karpovich", 1, 3, 4444, gr1, gr2);
    second student2("Kazantseva", 1, 2, 5555, gr3, gr4);
    std::cout << student1;
    std::cout << student2 << std::endl;
    Student* students[3];
    students[0] = new first("Sigma", 1, 2, 12345, gr5);
    students[1] = new second("Tyler Durden", 1, 2, 666, gr3, gr4);
    students[2] = new second("Kafka", 1, 2, 777, gr1, gr6);
    double lol = 0;
    for(int i = 0; i < 3; i++){
      std::cout << *students[i] << std::endl;
      lol += students[i] -> average();
      delete students[i];
    }
    lol /= 3;
    std::cout << "Average grade of group: " << lol << std::endl;
    return 0;
}