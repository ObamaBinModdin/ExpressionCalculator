#include <iostream>
#include "ExpressionSolver.h"

int main()
{  
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
            std::cout << "Answer: " << std::fixed << answer << std::endl << std::endl;
        }
        catch (std::invalid_argument& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch (std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
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