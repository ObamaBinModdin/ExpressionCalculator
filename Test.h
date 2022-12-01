#pragma once

#include <iostream>
#include <limits>
#include "exprtk.hpp"
#include "ExpressionSolver.h"
#include <iomanip>
#include <thread>


class test
{
public:
    test(long double numberOfExpressions, bool falseStatements = false);

private:
    static void generateTest(long double& numberOfIncorrectExpressions);
    static long double oracleEvaluator(std::string expression);
    static long double implementedEvaluator(std::string expression);
    static bool correct(long double oracleSolution, long double implementedSolution);
    static std::string generateCorrectExpression(int numberOfOperators = 2);
    static std::string generateFunction();
    static std::string generateOperation();
};
