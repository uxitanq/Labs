#ifndef lab6_h
#define lab6_h

#include <iostream>


class fraction {
    private: 
    int a; //num
    int b; //den

    void simplify();

    public: 
    fraction (int num, int den);
    fraction (const fraction& other);
    fraction() = delete;

    int getNum() const;
    int getDen() const;

    fraction operator+(const fraction& other) const;
    fraction operator*(const fraction& other) const;
    fraction operator/(const fraction& other) const;

    void print() const;
};

#endif