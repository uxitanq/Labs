#include "pch.h"
#include "Number.h"

Number::Number(double val) : value(val) {}

Number Number::operator+(const Number& other) const {
    return Number(value + other.value);
}

Number Number::operator-(const Number& other) const {
    return Number(value - other.value);
}

Number Number::operator*(const Number& other) const {
    return Number(value * other.value);
}

Number Number::operator/(const Number& other) const {
    if (other.value == 0.0) {
        throw "Division by zero!";
    }
    return Number(value / other.value);
}

double Number::getValue() const {
    return value;
}

void Number::setValue(double val) {
    value = val;
}

bool Number::operator==(const Number& other) const {
    return value == other.value;
}

Number ZERO(0.0);
Number ONE(1.0);

Number createNumber(double value) {
    return Number(value);
}