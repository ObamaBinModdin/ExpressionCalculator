#pragma once

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <regex>

/*
* Functions to solve a mathematical expression. All functions except for solve are helper functions.
* - Call solve() and pass in a mathematical expression of type string.
* - If a valid expression then an answer of type long double is returned.
*/
class expressionSolver
{
public:
    /*
    * Solves the mathematical expression for a given string.
    * - Pass a string of a valid mathematical expression.
    * - Returns a long double, the answer, if the expression can be solved. 
    */
    static long double solve(std::string& expression);

private:
    static std::string convertInfixToPostfix(std::string infix);

    static bool isMathOperator(char token);

    static int orderOfOperations(char token);

    static long double evaluatePostfix(const std::string& postfix);

    static long double calculate(char mathOperator, long double value1, long double value2);

    static bool isNumeric(char value);

    static bool isNumeric(std::string value);

    static std::vector<std::string> split(const std::string& source, const std::string& delimiters = " ");

    static std::string replaceAll(const std::string& source, const std::string& oldValue, const std::string& newValue);

    static void replaceTrig(std::string& expression);

    static void flipOperands(int start, std::string& expression);
};