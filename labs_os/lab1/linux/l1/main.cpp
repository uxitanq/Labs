#include <iostream>
#include "Number.h"
#include "Vector.h"

using namespace std;

void demonstrateNumbers() {
    cout << "Number" << endl;
    
    cout << "zero: " << ZERO.getValue() << endl;
    cout << "one: " << ONE.getValue() << endl;
    
    Number a = createNumber(5.0);
    Number b = createNumber(3.0);
    
    cout << "a = " << a.getValue() << ", b = " << b.getValue() << endl;
    
    Number sum = a + b;
    Number diff = a - b;
    Number product = a * b;
    Number quotient = a / b;
    
    cout << "a + b = " << sum.getValue() << endl;
    cout << "a - b = " << diff.getValue() << endl;
    cout << "a * b = " << product.getValue() << endl;
    cout << "a / b = " << quotient.getValue() << endl;
    
    cout << endl;
}

void demonstrateVectors() {
    cout << "vector" << endl;
    
    cout << "zero_vector: ";
    ZERO_VECTOR.print();
    
    cout << "one_one_vector: ";
    ONE_ONE_VECTOR.print();
    
    Vector v1(createNumber(3.0), createNumber(4.0));
    Vector v2(createNumber(1.0), createNumber(2.0));
    
    cout << "Vector v1: ";
    v1.print();
    
    cout << "Vector v2: ";
    v2.print();
    
    Vector v3 = v1 + v2;
    cout << "v1 + v2 = ";
    v3.print();
    
    Vector v4(createNumber(1.0), createNumber(1.0));
    cout << "Vector (1,1) in polar coordinates:" << endl;
    cout << "  Radius: " << v4.getRadius().getValue() << endl;
    cout << "  Angle: " << v4.getAngle().getValue() << " radians" << endl;
    
    Vector v5(createNumber(-2.0), createNumber(2.0));
    cout << "Vector (-2,2): ";
    v5.print();
}

int main() {
    try {
        demonstrateNumbers();
        demonstrateVectors();
        
        cout << "\n Success" << endl;
        
    } catch (const char* error) {
        cout << "Error: " << error << endl;
        return 1;
    }
    
    return 0;
}