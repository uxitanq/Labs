#pragma once

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

#include "Number.h"

class VECTOR_API Vector {
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

extern VECTOR_API Vector ZERO_VECTOR;
extern VECTOR_API Vector ONE_ONE_VECTOR;