#include <iostream>
#include "D:\labs\l6\lab6.h"

int main(){
    fraction f1(3, 4);
    fraction f2(1, 2);

    std::cout << "f1: ";
    f1.print();

    std::cout << "f2: ";
    f2.print();

    fraction f3 = f1 + f2;
    std::cout << "f1 + f2: ";
    f3.print();

    fraction f4 = f1 * f2;
    std::cout << "f1 * f2: ";
    f4.print();

    fraction f5 = f1 / f2;
    std::cout << "f1 / f2: ";
    f5.print();

    return 0;
}