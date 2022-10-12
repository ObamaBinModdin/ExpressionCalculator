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
    /*
    * Converts and infix expression to postfix.
    * - Pass an infix string.
    * - Returns a postfix expression of type string.
    */
    static std::string convertInfixToPostfix(std::string infix);

    /*
    * Determines if a char is an operator.
    * - Pass in a char.
    * - Returns true if an operator.
    */
    static bool isMathOperator(char token);

    /*
    * Determines the priority of an operator.
    * - Pass in an operator of type char.
    * - Returns an int of its priority.
    */
    static int orderOfOperations(char token);

    /*
    * Solves a postfix expression.
    * - Pass in a postfix expression of type string.
    * - Returns a long double of nthe answer to the postfix expression.
    */
    static long double evaluatePostfix(const std::string& postfix);

    /*
    * Solves one operation.
    * - Pass an operator and two values of type long double.
    * - Returns the answer of type long double.
    */
    static long double calculate(char mathOperator, long double value1, long double value2);

    /*
    * Determines if a char is a number.
    * - Pass in a value of type char.
    * - Returns true if a number.
    */
    static bool isNumeric(char value);

    /*
    * Determines if a char is a number.
    * - Pass in a value of type char.
    * - Returns true if a number.
    */
    static bool isNumeric(std::string value);

    /*
    * Splits a string to split numbers not connected.
    * - Pass a source string.
    * - Returns a vector of type string.
    */
    static std::vector<std::string> split(const std::string& source, const std::string& delimiters = " ");

    /*
    * Replaces all oldValues to newValue.
    * - Pass a source string, an oldValue string, and a newValue string.
    * - Returns a string of the expression after replacing oldValues.
    */
    static std::string replaceAll(const std::string& source, const std::string& oldValue, const std::string& newValue);

    /*
    * Replaces all trig functions to a single character.
    * - Pass the expression to replace trigs in.
    * - Return the expression after all trigs have been replaced.
    */
    static void replaceTrig(std::string& expression);

    /*
    * Flip operations after negating
    * - Pass the starting index and the expression.
    */
    static void flipOperators(int start, std::string& expression);
};