#include "Vector.h"
#include <cmath>
#include <iostream>

using namespace std;

Vector::Vector(const Number& x_val, const Number& y_val) 
    : x(x_val), y(y_val) {}

Number Vector::getX() const {
    return x;
}

Number Vector::getY() const {
    return y;
}

void Vector::setX(const Number& x_val) {
    x = x_val;
}

void Vector::setY(const Number& y_val) {
    y = y_val;
}

Number Vector::getRadius() const {
    Number x_sqr = x * x;
    Number y_sqr = y * y;
    Number sum = x_sqr + y_sqr;
    return createNumber(sqrt(sum.getValue()));
}

Number Vector::getAngle() const {
    return createNumber(atan2(y.getValue(), x.getValue()));
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x + other.x, y + other.y);
}

void Vector::print() const {
    cout << "Vector(" << x.getValue() << ", " << y.getValue() << ")" << endl;
    cout << "  Polar: r=" << getRadius().getValue() 
         << ", angle=" << getAngle().getValue() << " rad" << endl;
}

Vector ZERO_VECTOR(ZERO, ZERO);
Vector ONE_ONE_VECTOR(ONE, ONE);