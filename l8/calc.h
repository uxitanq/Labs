#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include "stack.h" 


class Calculator {
private:
    std::string expression;

    std::string removeSpaces(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (!std::isspace(c)) {
                result += c;
            }
        }
        return result;
    }

    bool isValidExpression(const std::string& expr) {
        int proverka = 0;
        for (char c : expr) {
            if (c == '(')
                proverka++;
            else if (c == ')')
                proverka--;
        }
        if (proverka != 0)
            return false;
        return true;
    }

    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    int Prioritet(char op) {
        if (isOperator(op)) {
            if (op == '+' || op == '-')
                return 1;
            return 2;
        }
        return 0;
    }

    double calc(const std::string& input) {
        std::string result;
        Stack operators;

        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            if (isdigit(c) || (c == '-' && (i == 0 || !isdigit(input[i - 1]) && input[i-1] != ')'))) { 
                std::string num;
                if (c == '-') {
                    num += '-';
                    i++;
                    while (i < input.length() && (isdigit(input[i]) || input[i] == '.')) {
                        num += input[i];
                        i++;
                    }
                    i--;
                }
                else {
                    while (i < input.length() && (isdigit(input[i]) || input[i] == '.')) {
                        num += input[i];
                        i++;
                    }
                    i--;
                }
                result += num + " ";
            }
            else if (c == '(')
                operators << (double)c;
            else if (c == ')') {
                double top;
                while (!operators.isEmpty()) {
                    operators >> top;
                    if ((char)top == '(')
                        break;
                    result += (char)top;
                    result += " ";
                }
            }
            else if (isOperator(c)) {
                while (!operators.isEmpty()) {
                    double top;
                    operators >> top;
                    if ((char)top == '(' || Prioritet(c) > Prioritet((char)top)) {
                        operators << top;
                        break;
                    }
                    result += (char)top;
                    result += " ";

                }
                operators << (double)c;
            }
            else if (!std::isspace(c))
                throw std::runtime_error("Mistake");
        }

        while (!operators.isEmpty()) {
            double top;
            operators >> top;
            result += (char)top;
            result += " ";
        }

        Stack stack;
        std::string s;
        for (size_t i = 0; i < result.length();) {
            while (i < result.length() && !std::isspace(result[i])) {
                s += result[i];
                i++;
            }
            i++;
            if (isdigit(s[0]) || (s[0] == '-' && s.size() > 1))
                stack << std::stod(s);
            else if (isOperator(s[0])) {
                if (stack.size() < 2)
                    throw std::runtime_error("Mistake");
                double a, b;
                stack >> a;
                stack >> b;
                switch (s[0]) {
                case '+':
                    stack << b + a;
                    break;
                case '-':
                    stack << b - a;
                    break;
                case '*':
                    stack << b * a;
                    break;
                case '/':
                    if (a == 0)
                        throw std::runtime_error("Division by zero");
                    stack << b / a;
                    break;
                }
            }
                s = "";
        }
        if(stack.size() != 1)
            throw std::runtime_error("Mistake");
        double finalResult;
        stack >> finalResult;
        return finalResult;
    }
public:
    Calculator(const std::string& expression) {
        this->expression = expression;
    }

    double calculate() {
        if (!isValidExpression(expression)) {
           throw std::runtime_error("Incorrect expression");
        }
        expression = removeSpaces (expression);
        return calc(expression);
    }
};