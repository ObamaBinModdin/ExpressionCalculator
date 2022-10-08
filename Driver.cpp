#include <iostream>
#include "ExpressionSolver.h"

int main()
{
    //std::string str = "--5^2/    ------(5*6)+2";
    //std::string str = "4 - 3";
    std::string str = "-5.78+-(4-2.23)+sin(0)*cos(1)/(1+tan(2*ln(-3+2*(1.23+99.111))))";
    long double answer = expressionSolver::solve(str);

    std::cout << answer;

    // FIX add/sub and negating
}  

/* Wrong Answers...
* cot(12) - 3
* -1 - 2
* -5.78+-(4-2.23)+sin(0)*cos(1)/(1+tan(2*ln(-3+2*(1.23+99.111))))
*/