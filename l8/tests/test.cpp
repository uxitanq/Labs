#include "pch.h"
#include "D:\labs\l8\calc.h" 

TEST(CalculatorTest, PositiveNumbers) {
    Calculator calc("5 + 3");
    ASSERT_DOUBLE_EQ(calc.calculate(), 8.0);
}

TEST(CalculatorTest, NegativeNumbers) {
    Calculator calc("-5 + 3");
    ASSERT_DOUBLE_EQ(calc.calculate(), -2.0);
}

TEST(CalculatorTest, Subtraction) {
    Calculator calc("10 - 4");
    ASSERT_DOUBLE_EQ(calc.calculate(), 6.0);
}

TEST(CalculatorTest, Multiplication) {
    Calculator calc("6 * 7");
    ASSERT_DOUBLE_EQ(calc.calculate(), 42.0);
}

TEST(CalculatorTest, Division) {
    Calculator calc("10 / 2");
    ASSERT_DOUBLE_EQ(calc.calculate(), 5.0);
}

TEST(CalculatorTest, DivisionByZero) {
    Calculator calc("10 / 0");
    ASSERT_THROW(calc.calculate(), std::runtime_error);
}

TEST(CalculatorTest, NegativeResult) {
    Calculator calc("3 - 5");
    ASSERT_DOUBLE_EQ(calc.calculate(), -2.0);
}

TEST(CalculatorTest, MixedOperations) {
    Calculator calc("5 + 2 * 3");
    ASSERT_DOUBLE_EQ(calc.calculate(), 11.0);
}

TEST(CalculatorTest, Parentheses) {
    Calculator calc("(5 + 2) * 3");
    ASSERT_DOUBLE_EQ(calc.calculate(), 21.0);
}
TEST(CalculatorTest, ParenthesesWithNegatives) {
     Calculator calc("2*(-5+3)");
    ASSERT_DOUBLE_EQ(calc.calculate(), -4.0);
}

TEST(CalculatorTest, UnaryMinus) {
    Calculator calc("-5 + 3");
    ASSERT_DOUBLE_EQ(calc.calculate(), -2.0);
}

TEST(CalculatorTest, DoubleUnaryMinus) {
     Calculator calc("5 - -3");
     ASSERT_DOUBLE_EQ(calc.calculate(), 8.0);
}

TEST(CalculatorTest, ComplexUnaryMinus) {
     Calculator calc("(-5+3)-(-6)");
     ASSERT_DOUBLE_EQ(calc.calculate(), 4.0);
}

TEST(CalculatorTest, NestedParentheses) {
     Calculator calc("((1 + 2) * 3) + 4");
     ASSERT_DOUBLE_EQ(calc.calculate(), 13.0);
}


TEST(CalculatorTest, NegativeParentheses) {
     Calculator calc("-(1+1)");
     ASSERT_DOUBLE_EQ(calc.calculate(), -2.0);
}
TEST(CalculatorTest, InvalidExpression) {
    Calculator calc("5 +");
    ASSERT_THROW(calc.calculate(), std::runtime_error);
    Calculator calc2("(5+2");
    ASSERT_THROW(calc2.calculate(), std::runtime_error);
}
TEST(CalculatorTest, DoubleNumbers) {
    Calculator calc("2.5 + 2.5");
    ASSERT_DOUBLE_EQ(calc.calculate(), 5.0);
}
TEST(CalculatorTest, NegativeDoubleNumbers) {
     Calculator calc("2.5 * -2");
     ASSERT_DOUBLE_EQ(calc.calculate(), -5.0);
}
TEST(CalculatorTest, DoubleNegativeDoubleNumbers) {
    Calculator calc("-2.5 - -2.5");
    ASSERT_DOUBLE_EQ(calc.calculate(), 0.0);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
