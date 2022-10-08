#include <iostream>
#include "ExpressionSolver.h"

int main()
{  
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

    while (getline(std::cin, userInputExpression) && userInputExpression != "quit")
    {
        std::cout << std::endl;



        try
        {
            std::string original = userInputExpression;
            long double answer = expressionSolver::solve(userInputExpression);

            // std::fixed prevents scientific notation. 
            if (isinf(answer))
            {
                std::cout << "Answer: " << std::fixed << original << std::endl << std::endl;
            }
            else
            {
                std::cout << "Answer: " << std::fixed << answer << std::endl << std::endl;
            }
        }
        catch (std::invalid_argument& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch (std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "AN UNKNOWN ERROR HAS OCCURRED" << std::endl;
        }

        std::cout << "========================================================================================" << std::endl;
        std::cout << "Type 'quit' to exit or enter another expression to calculate." << std::endl;
        std::cout << "========================================================================================" << std::endl << std::endl;

        std::cout << "Input: ";
    }


    //std::string str = "3/0";
    //long double answer = expressionSolver::solve(str);

    //std::cout << answer;

    // FIX add/sub and negating
}  

/* Wrong Answers...
* cot(12) - 3
* -1 - 2
* -5.78+-(4-2.23)+sin(0)*cos(1)/(1+tan(2*ln(-3+2*(1.23+99.111))))
*/