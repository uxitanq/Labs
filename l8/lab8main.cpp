
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include "calc.h"

int main() {
    std::string s;
    std::getline (std::cin, s);
    Calculator calc(s);
    std::cout << calc.calculate() << std::endl;
    return 0;
}