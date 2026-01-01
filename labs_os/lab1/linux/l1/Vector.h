#ifndef VECTOR_H
#define VECTOR_H

#include "Number.h"

class Vector {
private:
    Number x;
    Number y;

public:
    Vector(const Number& x_val = ZERO, const Number& y_val = ZERO);
    
    Number getX() const;
    Number getY() const;
    void setX(const Number& x_val);
    void setY(const Number& y_val);
    
    Number getRadius() const;    
    Number getAngle() const;    
    
    Vector operator+(const Vector& other) const;
    
    void print() const;
};

extern Vector ZERO_VECTOR;
extern Vector ONE_ONE_VECTOR;

#endif 