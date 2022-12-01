#include "Test.h"

/*
* Constructor for Test. Takes in a long double for numberOfExpressions and a boolean for falseStatements.
* Generates an expression, solve it with my implemented calculator, then calculate with the oracle ExprtK.
* Then will compare before outputting all above information and if both solutions were the same.
*/
test::test(long double numberOfExpressions, bool falseStatements)
{
    if (numberOfExpressions < 10) throw std::invalid_argument("ERROR: Number of expressions must be greater than zero.\n");

    long double numberOfIncorrectExpressions = 0;


    for (long double iterator = 0; iterator < numberOfExpressions; ++iterator)
    {
        generateTest(numberOfIncorrectExpressions);
    }

    std::cout << std::setw(35) << std::setfill('-') << "" << std::endl;
    std::cout << "Cases passed: " << numberOfExpressions - numberOfIncorrectExpressions << " / " << numberOfExpressions << " or "
        << ((numberOfExpressions - numberOfIncorrectExpressions) / numberOfExpressions) * 100.0 << "%" << std::endl;
}

/*
* Generates expressions and compares both calculations. Much like the constructor.
*/
void test::generateTest(long double &numberOfIncorrectExpressions)
{
    std::string expression;

    size_t randomInt = rand() % 10 + 1;

    expression = generateCorrectExpression(randomInt);

    long double calcAnswer = oracleEvaluator(expression);
    long double oracleAnswer = implementedEvaluator(expression);

    if (correct(calcAnswer, oracleAnswer))
    {
        std::cout << "Passed | ";
    }
    else
    {
        numberOfIncorrectExpressions++;
        std::cout << "Failed | ";
    }

    std::cout << std::left << std::setw(100) << expression;

    if (calcAnswer != std::numeric_limits<long double>::min())
    {
        std::cout << std::left << std::setw(0) << "Calculator Result: " << std::left << std::setw(25) << calcAnswer;
    }
    else
    {
        std::cout << std::left <<  std::setw(0) << "Calculator Result: " << std::left << std::setw(25) << "ERROR";
    }

    if (oracleAnswer != std::numeric_limits<long double>::min())
    {
        std::cout << std::left << std::setw(0) << "Oracle Result: " << std::left << std::setw(25) << oracleAnswer << std::endl;
    }
    else
    {
        std::cout << std::left << std::setw(0) << "Oracle Result: " << std::left << std::setw(25) << "ERROR" << std::endl;
    }
}

/*
* Takes the mathematical expression as a string. Returns the long double solution after the expression is passed into the ExprtK library.
*/
long double test::oracleEvaluator(std::string mathematicalExpression)
{
    typedef long double T;

    typedef exprtk::symbol_table<T> symbolTableT;
    typedef exprtk::expression<T> expressionT;
    typedef exprtk::parser<T> parserT;

    mathematicalExpression = "x := " + mathematicalExpression;

    T x = T(0.0);

    symbolTableT symbolTable;
    symbolTable.add_variable("x", x);

    expressionT expression;
    expression.register_symbol_table(symbolTable);

    parserT parser;

    if (!parser.compile(mathematicalExpression, expression))
    {
        return std::numeric_limits<long double>::min();
    }

    T result = expression.value();

    if (isinf(result))
    {
        return std::numeric_limits<long double>::min();
    }

    return result;
}

/*
* Calls the implemented calculator and passes in the expression before returning the solution as a long double.
*/
long double test::implementedEvaluator(std::string expression)
{
    try
    {
        return expressionSolver::solve(expression);
    }
    catch (...)
    {
        return std::numeric_limits<long double>::min();
    }
}

/*
* Compares two solutions and returns true if both are the same.
*/
bool test::correct(long double oracleSolution, long double implementedSolution)
{
    return oracleSolution == implementedSolution;
}

/*
* Creates a valid expression and stops when the number of operators has been reached. Returns the expression.
*/
std::string test::generateCorrectExpression(int numberOfOperators)
{
    std::string expression = std::string();
    size_t randomInt = rand() % 3;
    int previousValue = -1;
    bool decimalFlag = false;
    std::stack<char> bracketStack;

    while (true)
    {
        if (numberOfOperators <= 0 && std::isdigit(expression[expression.size() - 1]))
        {
            while (bracketStack.size() != 0)
            {
                expression += bracketStack.top();
                bracketStack.pop();
            }
            break;
        }
        // Starting path
        else if (previousValue == -1)
        {
            randomInt = rand() % 5;

            if (randomInt == 0)
            {
                previousValue = -1;
                decimalFlag = false;

                bracketStack.push(')');
                expression += "(";
            }
            else if (randomInt == 1)
            {
                previousValue = -1;
                decimalFlag = false;
                expression += "-";
            }
            else if (randomInt == 2)
            {
                previousValue = 0;
                expression += std::to_string(rand() % 10);
            }
            else if (randomInt == 3 && decimalFlag == false)
            {
                previousValue = 1;
                decimalFlag = true;

                if (expression.size() > 0 && !std::isdigit(expression[expression.size() - 1]))
                {
                    expression += "0";
                }

                expression += ".";
            }
            else
            {
                previousValue = 1;
                decimalFlag = false;
                bracketStack.push(')');
                expression += generateFunction();
                numberOfOperators--;
            }
        }
        else if (previousValue == 0)
        {
            randomInt = rand() % 5;

            if (randomInt == 1 && decimalFlag == false)
            {
                previousValue = 1;
                decimalFlag = true;

                if (expression.size() > 0 && !std::isdigit(expression[expression.size() - 1]))
                {
                    expression += "0";
                }

                expression += ".";
            }
            else if (randomInt == 2)
            {
                previousValue = -1;
                decimalFlag = false;
                numberOfOperators--;
                
                std::string expressionOperator = generateOperation();

                if (expression[0] == '^')
                {
                    expression += "^";
                    expression += "(";
                    bracketStack.push(')');
                }
                else
                {
                    expression += expressionOperator;
                }
            }
            else if (randomInt == 3 && bracketStack.size() != 0)
            {
                previousValue = -1;
                if (numberOfOperators > 0)
                {
                    previousValue = 2;
                }

                expression += bracketStack.top();
                bracketStack.pop();
            }
            else
            {
                previousValue = 0;
                expression += std::to_string(rand() % 10);
            }

        }
        else if (previousValue == 1)
        {
            previousValue = 0;
            expression += std::to_string(rand() % 10);
        }
        else
        {
            previousValue = -1;
            decimalFlag = false;
            numberOfOperators--;

            std::string expressionOperator = generateOperation();

            if (expression[0] == '^')
            {
                expression += "^";
                expression += "(";
                bracketStack.push(')');
            }
            else
            {
                expression += expressionOperator;
            }
        }
    }

    return expression;
}

/*
* Based on a random chance of 6. One of the six functions will be selected and returned.
*/
std::string test::generateFunction()
{
    switch (rand() % 6)
    {
    case 0:
        return "cos(";
    case 1:
        return "sin(";
    case 2:
        return "cot(";
    case 3:
        return "ln(";
    case 4:
        return "log10(";
    default:
        return "tan(";
    }
}

/*
* Chooses one operator of 5. Returns the chosen operator.
*/
std::string test::generateOperation()
{
    switch (rand() % 5)
    {
    case 0:
        return "+";
    case 1:
        return "-";
    case 2:
        return "*";
    case 3:
        return "/";
    default:
        return "^";
    }
}
