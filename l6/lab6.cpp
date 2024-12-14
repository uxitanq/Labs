#include "lab6.h"
#include <numeric>
#include <iostream>

fraction::fraction (int num, int den) : a(num), b (den){
    if (den == 0){
        std::cout << "Знаменатель равен 0" << std::endl;
        exit (0);
    }

    simplify();
}

fraction::fraction(const fraction& other) : a(other.a), b(other.b) {
    simplify(); 
}

int fraction::getNum() const {
    return a;
}

int fraction::getDen() const {
    return b;
}

void fraction::simplify() {
    if (b == 0) {
        std::cerr << "Знаменатель равен нулю" << std::endl;
        exit(1); 
    }
    int lgcd = std::gcd(a, b);
    a /= lgcd;
    b /= lgcd;
}

fraction fraction::operator+(const fraction& other) const {
    int newa = a * other.b + b * other.a;
    int newb = b * other.b;
    return fraction(newa, newb);
}

fraction fraction::operator*(const fraction& other) const {
    int newa = a * other.a;
    int newb = b * other.b;
    return fraction(newa, newb);
}


fraction fraction::operator/(const fraction& other) const {
    if (other.a == 0){
        std::cout << "Деление на 0" << std::endl;
        exit (1);
    }
    int newa = a * other.b;
    int newb = b * other.a;
    return fraction(newa, newb);
}

void fraction::print() const{
   if (a == 0) {
        std::cout << "0" << std::endl;
        return;
    }
    if (b == 1) {
        std::cout << a << std::endl;
        return;
    }
    std::cout << a << "/" << b << std::endl;
}