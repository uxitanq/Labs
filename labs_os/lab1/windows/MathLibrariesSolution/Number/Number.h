#pragma once

class Number {
private:
    double value;

public:
    Number(double val = 0.0);

    Number operator+(const Number& other) const;
    Number operator-(const Number& other) const;
    Number operator*(const Number& other) const;
    Number operator/(const Number& other) const;

    double getValue() const;
    void setValue(double val);

    bool operator==(const Number& other) const;
};

extern Number ZERO;
extern Number ONE;

Number createNumber(double value);