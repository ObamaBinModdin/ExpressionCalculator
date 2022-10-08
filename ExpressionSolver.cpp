#include "ExpressionSolver.h"

/*
* 
*/
long double expressionSolver::solve(std::string& expression)
{
    if (expression.empty()) throw std::invalid_argument("ERROR: NO INPUT\n");

    if (std::find_if(expression.begin(), expression.end(), ::isdigit) == expression.end())
    {
        throw std::invalid_argument("ERROR: NO NUMBERS WERE INPUTTED\n");
    }

    replaceTrig(expression);

    expression = convertInfixToPostfix(expression);

    return evaluatePostfix(expression);
}

std::string expressionSolver::convertInfixToPostfix(std::string infix) {
    // this function converts an infix expression to postfix
    // declare function variables
    std::string postfix;
    std::stack<char> charStack;

    // remove all whitespace from the string
    infix.erase(std::remove_if(infix.begin(), infix.end(), [](char c) 
        {
        return std::isspace(static_cast<unsigned char>(c));
        }), infix.end());

    if (infix[infix.length() - 1] == '=')
    {
        infix.pop_back();

        if (infix.find('=') != std::string::npos)
        {
            //std::cout << "ERROR: INVALID '=' PLACEMENT" << std::endl;
            throw std::invalid_argument{ "ERROR: INVALID '=' PLACEMENT\n" };
        }
    }

    // negate equations marked with '--'
    infix = replaceAll(infix, "--", "");


    // automatically convert negative numbers to have the ~ symbol.
    // this is done so we can distinguish negative numbers and the subtraction symbol
    for (unsigned x = 0; x < infix.length(); ++x) 
    {
        if (infix[x] != '-') 
        {
            continue;
        }
        if (x == 0 || infix[x - 1] == '(' || isMathOperator(infix[x - 1])) 
        {
            infix[x] = '~';
            flipOperands(x, infix);
        }
    }


    // loop thru array until there is no more data
    for (unsigned x = 0; x < infix.length(); ++x) 
    {
        // place numbers (standard, decimal, & negative)
        // numbers onto the 'postfix' string
        if (isNumeric(infix[x])) 
        {
            if (postfix.length() > 0 && !isNumeric(postfix.back())) 
            {
                if (!std::isspace(postfix.back())) 
                {
                    postfix += " ";
                }
            }
            postfix += infix[x];

        }
        else if (std::isspace(infix[x])) 
        {
            continue;

        }
        else if (isMathOperator(infix[x])) 
        {
            if (postfix.length() > 0 && !std::isspace(postfix.back())) 
            {
                postfix += " ";
            }
            // use the 'orderOfOperations' function to check equality
            // of the math operator at the top of the stack compared to
            // the current math operator in the infix string
            while ((!charStack.empty()) &&
                (orderOfOperations(charStack.top()) >= orderOfOperations(infix[x]))) {
                // place the math operator from the top of the
                // stack onto the postfix string and continue the
                // process until complete
                if (postfix.length() > 0 && !std::isspace(postfix.back())) 
                {
                    postfix += " ";
                }
                postfix += charStack.top();
                charStack.pop();
            }
            // push the remaining math operator onto the stack
            charStack.push(infix[x]);
        }
        // push outer parentheses onto stack
        else if (infix[x] == '(') 
        {
            charStack.push(infix[x]);
        }
        else if (infix[x] == ')') {
            // pop the current math operator from the stack
            while ((!charStack.empty()) && (charStack.top() != '(')) {
                if (postfix.length() > 0 && !std::isspace(postfix.back())) {
                    postfix += " ";
                }
                // place the math operator onto the postfix string
                postfix += charStack.top();
                // pop the next operator from the stack and
                // continue the process until complete
                charStack.pop();
            }

            // pop '(' symbol off the stack
            if (!charStack.empty()) {
                charStack.pop();
            }
            else {
                // no matching '('
                throw std::invalid_argument{ "ERROR: PARENTHESES MISMATCH\n" };
            }
        }
        else {
            throw std::invalid_argument{ "ERROR: INVALID INPUT\n" };
        }
    }

    // place any remaining math operators from the stack onto
    // the postfix array
    while (!charStack.empty()) {
        if (charStack.top() == '(' || charStack.top() == ')') {
            throw std::invalid_argument{ "ERROR: PARENTHESES MISMATCH\n" };
        }
        if (postfix.length() > 0 && !std::isspace(postfix.back())) {
            postfix += " ";
        }
        postfix += charStack.top();
        charStack.pop();
    }

    // replace all '~' symbols with a minus sign
    postfix = replaceAll(postfix, "~", "-");

    return postfix;
}// end of convertInfixToPostfix

bool expressionSolver::isMathOperator(char token) {
    // this function checks if operand is a math operator
    switch (std::tolower(token)) {
    case '+': case '-': case '*': case '/':
    case '%': case '^': case 'c':
    case 's': case 't': case 'v': case 'l': case 'n':
        return true;
        break;
    default:
        return false;
        break;
    }
}// end of isMathOperator

int expressionSolver::orderOfOperations(char token) {
    // this function returns the priority of each math operator
    int priority = 0;
    switch (std::tolower(token)) {
    case 'c': case 's': case 't': case 'v': case 'l': case 'n':
        priority = 5;
        break;
    case '^':
        priority = 4;
        break;
    case '*': case '/': case '%':
        priority = 3;
        break;
    case '-':
        priority = 2;
        break;
    case '+':
        priority = 1;
        break;
    }
    return priority;
}// end of orderOfOperations

long double expressionSolver::evaluatePostfix(const std::string& postfix) {
    // this function evaluates a postfix expression
        // declare function variables
    long double answer = 0;
    std::stack<long double> doubleStack;

    // split string into tokens to isolate multi digit, negative and decimal
    // numbers, aswell as single digit numbers and math operators
    auto tokens = split(postfix);

    // display the found tokens to the screen
    //for (unsigned x = 0; x < tokens.size(); ++x) {
    //    std::cout<< tokens.at(x) << std::endl;
    //}


    // loop thru array until there is no more data
    for (unsigned x = 0; x < tokens.size(); ++x) {
        auto token = tokens[x];

        // push numbers & negative numbers onto the stack
        if (isNumeric(token)) {
            doubleStack.push(std::atof(token.c_str()));
        }
        // if expression is a math operator, pop numbers from stack
        // & send the popped numbers to the 'calculate' function
        else if (isMathOperator(token[0]) && (!doubleStack.empty())) {
            long double value1 = 0;
            long double value2 = 0;
            char mathOperator = static_cast<unsigned char>(std::tolower(token[0]));

            // if expression is square root, sin, cos,
            // or tan operation only pop stack once
            if (mathOperator == 's' || mathOperator == 'c' || mathOperator == 't' || mathOperator == 'v'
                || mathOperator == 'l' || mathOperator == 'n') {
                value2 = 0;
                value1 = doubleStack.top();
                doubleStack.pop();
                answer = calculate(mathOperator, value1, value2);
                doubleStack.push(answer);

            }
            else if (doubleStack.size() > 1) {
                value2 = doubleStack.top();
                doubleStack.pop();
                value1 = doubleStack.top();
                doubleStack.pop();
                answer = calculate(mathOperator, value1, value2);
                doubleStack.push(answer);
            }
        }
        else {
            // this should never execute, & if it does, something went really wrong
            throw std::invalid_argument{ "ERROR: INVALID POSTFIX STRING\n" };
        }
    }
    // pop the final answer from the stack, and return to main
    if (!doubleStack.empty()) {
        answer = doubleStack.top();
    }
    return answer;
}// end of evaluatePostfix

long double expressionSolver::calculate(char mathOperator, long double value1, long double value2) {
    // this function carries out the actual math process
    long double ans = 0;
    switch (std::tolower(mathOperator)) {
    case '+':
        ans = value1 + value2;
        break;
    case '-':
        ans = value1 - value2;
        break;
    case '*':
        ans = value1 * value2;
        break;
    case '/':
        if (value2 == 0)
        {
            throw std::runtime_error("ERROR: ATTEMPTED TO DIVIDE BY ZERO\n");
        }

        ans = value1 / value2;
        break;
    case '%':
        ans = ((int)value1 % (int)value2) + std::modf(value1, &value2);
        break;
    case '^':
        ans = std::pow(value1, value2);
        break;
    case 'c':
        ans = std::cos(value1);
        break;
    case 's':
        ans = std::sin(value1);
        break;
    case 't':
        ans = std::tan(value1);
        break;
    case 'v':
        if (std::tan(value1) == 0) throw std::runtime_error("ERROR: COT WAS UNDEFINED\n");
        ans = 1 / std::tan(value1);
        break;
    case 'l':
        if (value1 < 0) throw std::runtime_error("ERROR: ATTEMPTED TO LOG A NEGATIVE\n");
        if (value1 == 0) throw std::runtime_error("ERROR: ATTEMPTED TO LOG ZERO\n");
        ans = std::log(value1);
        break;
    case 'n':
        if (value1 < 0) throw std::runtime_error("ERROR: ATTEMPTED TO LOG A NEGATIVE\n");
        if (value1 == 0) throw std::runtime_error("ERROR: ATTEMPTED TO LOG ZERO\n");
        ans = std::log10(value1);
        break;
    default:
        ans = 0;
        break;
    }
    return ans;
}// end of calculate

std::vector<std::string> expressionSolver::split(const std::string& source, const std::string& delimiters) {
    std::size_t prev = 0;
    std::size_t currentPos = 0;
    std::vector<std::string> results;

    while ((currentPos = source.find_first_of(delimiters, prev)) != std::string::npos) {
        if (currentPos > prev) {
            results.push_back(source.substr(prev, currentPos - prev));
        }
        prev = currentPos + 1;
    }
    if (prev < source.length()) {
        results.push_back(source.substr(prev, std::string::npos));
    }
    return results;
}// end of split

std::string expressionSolver::replaceAll(const std::string& source, const std::string& oldValue, const std::string& newValue) 
{
    if (oldValue.empty()) 
    {
        return source;
    }
    std::string newString;
    newString.reserve(source.length());
    std::size_t lastPos = 0;
    std::size_t findPos;
    while (std::string::npos != (findPos = source.find(oldValue, lastPos))) {
        newString.append(source, lastPos, findPos - lastPos);
        newString += newValue;
        lastPos = findPos + oldValue.length();
    }
    newString += source.substr(lastPos);
    return newString;
}// end of replaceAll

bool expressionSolver::isNumeric(char value) {
    return std::isdigit(value) || value == '.' || value == '~';
}// end of isNumeric

bool expressionSolver::isNumeric(std::string value) {
    for (unsigned index = 0; index < value.length(); ++index) {
        if (index == 0 && value[index] == '-' && value.length() > 1) {
            continue;
        }
        if (!isNumeric(value[index])) {
            return false;
        }
    }
    return true;
}// http://programmingnotes.org/

void expressionSolver::replaceTrig(std::string& expression)
{
    expression = std::regex_replace(expression, std::regex("sin"), "s");

    expression = std::regex_replace(expression, std::regex("cos"), "c");

    expression = std::regex_replace(expression, std::regex("tan"), "t");

    expression = std::regex_replace(expression, std::regex("cot"), "v");

    expression = std::regex_replace(expression, std::regex("ln"), "l");

    expression = std::regex_replace(expression, std::regex("log"), "n");
}

void expressionSolver::flipOperands(int start, std::string& expression)
{
    std::stack<char> charStack;

    for (int index = start + 1; index < expression.length(); index++)
    {
        if (expression[index] == '(') charStack.push('(');
        else if (expression[index] == '-') expression[index] = '+';
        else if (expression[index] == '+') expression[index] = '-';
        else if (expression[index] == ')') charStack.pop();

        if (charStack.empty()) break;
    }
}