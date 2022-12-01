#include <iostream>
#include "ExpressionSolver.h"
#include "Test.h"

/*
* Programmer: Oscar Ford
* 
* Credit: https://www.programmingnotes.org/6949/cpp-simple-multi-digit-decimal-negative-number-infix-to-postfix-conversion-evaluation/
*/

static void run();
static void test(long double numberOfExpressions = 100);

int main()
{
    long long input;

    std::cout << "Enter the number of expressions to evaluate: ";

    std::cin >> input;

    std::cout << std::endl;

    test(input);
}

void run()
{
    // Displays valid operations and the format to use them.
    std::cout << "Valid operations:" << std::endl;
    std::cout << "x + y to add" << std::endl;
    std::cout << "x - y to subtract" << std::endl;
    std::cout << "-x to negate" << std::endl;
    std::cout << "x * y to multiply" << std::endl;
    std::cout << "x / y to divide" << std::endl;
    std::cout << "x % y for modular" << std::endl;
    std::cout << "x ^ y for x to the power of y" << std::endl;
    std::cout << "cos(x) for cosine of x" << std::endl;
    std::cout << "sin(x) for sine of x" << std::endl;
    std::cout << "tan(x) for tanget of x" << std::endl;
    std::cout << "cot(x) for cotanget of x" << std::endl;
    std::cout << "ln(x) for natural log of x" << std::endl;
    std::cout << "log(x) for log of base 10 of x" << std::endl;
    std::cout << "() may be used at will but must be closed off" << std::endl;
    std::cout << std::endl << "----------------------------------------------------------------------------------------" << std::endl << std::endl;


    std::string userInputExpression;

    std::cout << "Enter a valid expression to calculate: ";

    // Get inputs from user until the user enters 'quit'.
    while (getline(std::cin, userInputExpression) && userInputExpression != "quit")
    {
        std::cout << std::endl;



        try
        {
            std::string original = userInputExpression;
            long double answer = expressionSolver::solve(userInputExpression);

            // std::fixed prevents scientific notation. 
            // If answer is 'inf' then display the original input. Otherwise display answer.
            if (std::isinf(answer))
            {
                std::cout << "Answer: " << std::fixed << original << std::endl << std::endl;
            }
            else
            {
                std::cout << "Answer: " << std::fixed << answer << std::endl << std::endl;
            }
        }
        // If an invalid_argument exception is thrown then print the details to console.
        catch (std::invalid_argument& e)
        {
            std::cerr << e.what() << std::endl;
        }
        // If an runtime_error exception is thrown then print the details to console.
        catch (std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
        }
        // If any uncaught exceptions are thrown then print 'unknown error' to console.
        catch (...)
        {
            std::cerr << "AN UNKNOWN ERROR HAS OCCURRED" << std::endl;
        }

        // Barriers between expressions.
        std::cout << "========================================================================================" << std::endl;
        std::cout << "Type 'quit' to exit or enter another expression to calculate." << std::endl;
        std::cout << "========================================================================================" << std::endl << std::endl;

        // Displays 'Input: ' for next user input.
        std::cout << "Input: ";
    }
}

void test(long double numberOfExpressions)
{



    test::test(numberOfExpressions);
}