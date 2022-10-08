#include "ExpressionSolver.h"

/*
* 
*/
long double expressionSolver::solve(std::string& expression)
{
    std::cout << expression << std::endl;

    replaceTrig(expression);
    std::cout << expression << std::endl;

    expression = convertInfixToPostfix(expression);
    std::cout << expression << std::endl;

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

    // negate equations marked with '--'
    infix = replaceAll(infix, "--", "");

    std::cout << infix << std::endl;

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

    std::cout << infix << std::endl;

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
                throw std::invalid_argument{ "PARENTHESES MISMATCH" };
            }
        }
        else {
            throw std::invalid_argument{ "INVALID INPUT" };
        }
    }

    // place any remaining math operators from the stack onto
    // the postfix array
    while (!charStack.empty()) {
        if (charStack.top() == '(' || charStack.top() == ')') {
            throw std::invalid_argument{ "PARENTHESES MISMATCH" };
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
    case '%': case '^': case '$': case 'c':
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
    case '^': case '$':
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

double expressionSolver::evaluatePostfix(const std::string& postfix) {
    // this function evaluates a postfix expression
        // declare function variables
    double answer = 0;
    std::stack<double> doubleStack;

    // split string into tokens to isolate multi digit, negative and decimal
    // numbers, aswell as single digit numbers and math operators
    auto tokens = split(postfix);

    // display the found tokens to the screen
    //for (unsigned x = 0; x < tokens.size(); ++x) {
    //    std::cout<< tokens.at(x) << std::endl;
    //}

    std::cout << "\nCalculations:\n";

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
            double value1 = 0;
            double value2 = 0;
            char mathOperator = static_cast<unsigned char>(std::tolower(token[0]));

            // if expression is square root, sin, cos,
            // or tan operation only pop stack once
            if (mathOperator == '$' || mathOperator == 's' || mathOperator == 'c' || mathOperator == 't' || mathOperator == 'v'
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
            throw std::invalid_argument{ "INVALID POSTFIX STRING" };
        }
    }
    // pop the final answer from the stack, and return to main
    if (!doubleStack.empty()) {
        answer = doubleStack.top();
    }
    return answer;
}// end of evaluatePostfix

double expressionSolver::calculate(char mathOperator, double value1, double value2) {
    // this function carries out the actual math process
    double ans = 0;
    switch (std::tolower(mathOperator)) {
    case '+':
        std::cout << value1 << mathOperator << value2;
        ans = value1 + value2;
        break;
    case '-':
        std::cout << value1 << mathOperator << value2;
        ans = value1 - value2;
        break;
    case '*':
        std::cout << value1 << mathOperator << value2;
        ans = value1 * value2;
        break;
    case '/':
        std::cout << value1 << mathOperator << value2;
        ans = value1 / value2;
        break;
    case '%':
        std::cout << value1 << mathOperator << value2;
        ans = ((int)value1 % (int)value2) + std::modf(value1, &value2);
        break;
    case '^':
        std::cout << value1 << mathOperator << value2;
        ans = std::pow(value1, value2);
        break;
    case '$':
        std::cout << char(251) << value1;
        ans = std::sqrt(value1);
        break;
    case 'c':
        std::cout << "cos(" << value1 << ")";
        ans = std::cos(value1);
        break;
    case 's':
        std::cout << "sin(" << value1 << ")";
        ans = std::sin(value1);
        break;
    case 't':
        std::cout << "tan(" << value1 << ")";
        ans = std::tan(value1);
        break;
    case 'v':
        std::cout << "cot(" << value1 << ")";
        ans = 1 / std::tan(value1);
        break;
    case 'l':
        std::cout << "log(" << value1 << ")";
        ans = std::log(value1);
        break;
    case 'n':
        std::cout << "log10(" << value1 << ")";
        ans = std::log10(value1);
        break;
    default:
        ans = 0;
        break;
    }
    std::cout << " = " << ans << std::endl;
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

//void expressionSolver::removeWhiteSpace(std::string& str)
//{
//    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
//}
//
//bool expressionSolver::isOperator(char& op)
//{
//    if (op == '+' || op == '-' || op == '*' || op == '/' || op == '_' || op == '^' || op == '!' || op == '@' || op == '$'
//        || op == '%' || op == '&' || op == '~') return true;
//
//    return false;
//}
//
//bool expressionSolver::isNumber(char& num)
//{
//    if (num >= '0' && num <= '9') return true;
//
//    return false;
//}
//
//bool expressionSolver::isNumeric(char value)
//{
//    return std::isdigit(value) || value == '.' || value == '~';
//}// end of isNumeric
//
//bool expressionSolver::isNumeric(std::string value)
//{
//    for (unsigned index = 0; index < value.length(); ++index) 
//    {
//        if (index == 0 && value[index] == '-' && value.length() > 1) 
//        {
//            continue;
//        }
//        if (!isNumeric(value[index])) 
//        {
//            return false;
//        }
//    }
//    return true;
//}// http://programmingnotes.org/
//
//double expressionSolver::evaluatePostfix(const std::string& postfix) 
//{
//    // this function evaluates a postfix expression
//        // declare function variables
//    double answer = 0;
//    std::stack<double> doubleStack;
//
//    // split string into tokens to isolate multi digit, negative and decimal
//    // numbers, aswell as single digit numbers and math operators
//    auto tokens = split(postfix);
//
//    // display the found tokens to the screen
//    //for (unsigned x = 0; x < tokens.size(); ++x) {
//    //    std::cout<< tokens.at(x) << std::endl;
//    //}
//
//    std::cout << "\nCalculations:\n";
//
//    // loop thru array until there is no more data
//    for (unsigned x = 0; x < tokens.size(); ++x) 
//    {
//        auto token = tokens[x];
//
//        // push numbers & negative numbers onto the stack
//        if (isNumeric(token)) 
//        {
//            doubleStack.push(std::atof(token.c_str()));
//        }
//        // if expression is a math operator, pop numbers from stack
//        // & send the popped numbers to the 'calculate' function
//        else if (isMathOperator(token[0]) && (!doubleStack.empty())) 
//        {
//            double value1 = 0;
//            double value2 = 0;
//            char mathOperator = static_cast<unsigned char>(std::tolower(token[0]));
//
//            // if expression is square root, sin, cos,
//            // or tan operation only pop stack once
//            if (mathOperator == '$' || mathOperator == 's' || mathOperator == 'c' || mathOperator == 't') 
//            {
//                value2 = 0;
//                value1 = doubleStack.top();
//                doubleStack.pop();
//                answer = calculate(mathOperator, value1, value2);
//                doubleStack.push(answer);
//
//            }
//            else if (doubleStack.size() > 1) 
//            {
//                value2 = doubleStack.top();
//                doubleStack.pop();
//                value1 = doubleStack.top();
//                doubleStack.pop();
//                answer = calculate(mathOperator, value1, value2);
//                doubleStack.push(answer);
//            }
//        }
//        else 
//        {
//            // this should never execute, & if it does, something went really wrong
//            throw std::invalid_argument{ "INVALID POSTFIX STRING" };
//        }
//    }
//    // pop the final answer from the stack, and return to main
//    if (!doubleStack.empty()) 
//    {
//        answer = doubleStack.top();
//    }
//    return answer;
//}// end of evaluatePostfix
//
//std::vector<std::string> expressionSolver::split(const std::string& source, const std::string& delimiters)
//{
//    std::size_t prev = 0;
//    std::size_t currentPos = 0;
//    std::vector<std::string> results;
//
//    while ((currentPos = source.find_first_of(delimiters, prev)) != std::string::npos) 
//    {
//        if (currentPos > prev) 
//        {
//            results.push_back(source.substr(prev, currentPos - prev));
//        }
//        prev = currentPos + 1;
//    }
//    if (prev < source.length()) 
//    {
//        results.push_back(source.substr(prev, std::string::npos));
//    }
//    return results;
//}// end of split
//
//int expressionSolver::precedence(char& op)
//{
//    if (op == '+') return 1;
//
//    else if (op == '-') return 2;
//
//    else if (op == '*' || op == '/' || op == '_') return 3;
//
//    else if (op == '^') return 4;
//
//    else if (op == '!' || op == '@' || op == '$' || op == '%' || op == '&' || op == '~') return 5;
//
//    return 0;
//}
//
//bool expressionSolver::equalOrHigher(char& opOne, char& opTwo)
//{
//    int precedenceOne = precedence(opOne);
//    int precedenceTwo = precedence(opTwo);
//
//    if (precedenceOne == precedenceTwo)
//    {
//        if (opOne == '^') return false;
//
//        return true;
//    }
//
//    return (precedenceOne > precedenceTwo ? true : false);
//}
//
//void expressionSolver::replaceTrig(std::string& expression)
//{
//    expression = std::regex_replace(expression, std::regex("sin"), "!");
//
//    expression = std::regex_replace(expression, std::regex("cos"), "@");
//
//    expression = std::regex_replace(expression, std::regex("tan"), "$");
//
//    expression = std::regex_replace(expression, std::regex("cot"), "%");
//
//    expression = std::regex_replace(expression, std::regex("ln"), "&");
//
//    expression = std::regex_replace(expression, std::regex("log"), "~");
//}
//
//std::string expressionSolver::replaceUnaryOperators(std::string& expression)
//{
//    for (int index = 0; index < expression.length(); index++)
//    {
//        if (index == 0 && expression[0] == '-' && !isNumber(expression[index])) expression[0] = '_';
//        else if (index > 0 && expression[index] == '-' && isOperator(expression[index - 1]) && !isNumber(expression[index])) expression[index] = '_';
//        else if (index > 0 && expression[index] == '-' && expression[index - 1] == '(' && !isNumber(expression[index])) expression[index] = '_';
//    }
//
//    return expression;
//}
//
//std::string expressionSolver::replaceAll(const std::string& source
//    , const std::string& oldValue, const std::string& newValue) 
//{
//    if (oldValue.empty()) 
//    {
//        return source;
//    }
//    std::string newString;
//    newString.reserve(source.length());
//    std::size_t lastPos = 0;
//    std::size_t findPos;
//    while (std::string::npos != (findPos = source.find(oldValue, lastPos))) 
//    {
//        newString.append(source, lastPos, findPos - lastPos);
//        newString += newValue;
//        lastPos = findPos + oldValue.length();
//    }
//    newString += source.substr(lastPos);
//    return newString;
//}// end of replaceAll
//
////std::string expressionSolver::toPostfix(std::string& infix)
////{
////    std::stack <char> stack;
////    std::string postfix = "";
////    char current;
////
////    stack.push('(');
////    infix += ')';
////
////    for (int index = 0; index < infix.length(); index++)
////    {
////        current = infix[index];
////
////        if (current == '(') stack.push(current);
////        else if (isNumber(current)) postfix += current;
////        else if (current == '.') postfix += current;
////        else if (current == ')')
////        {
////            while (!stack.empty() && stack.top() != '(')
////            {
////                postfix += stack.top();
////                stack.pop();
////            }
////
////            stack.pop();
////        }
////        else if (isOperator(current))
////        {
////            while (!stack.empty() && equalOrHigher(stack.top(), current))
////            {
////                postfix += stack.top();
////                stack.pop();
////            }
////
////            stack.push(current);
////        }
////    }
////
////    return postfix;
////}
//
//std::string expressionSolver::toPostfix(std::string& infix) 
//{
//    // this function converts an infix expression to postfix
//            // declare function variables
//    std::string postfix;
//    std::stack<char> charStack;
//
//    // remove all whitespace from the string
//    infix.erase(std::remove_if(infix.begin(), infix.end(), [](char c) 
//        {
//        return std::isspace(static_cast<unsigned char>(c));
//        }), infix.end());
//
//    // negate equations marked with '--'
//    infix = replaceAll(infix, "(--", "(");
//
//    // automatically convert negative numbers to have the ~ symbol.
//    // this is done so we can distinguish negative numbers and the subtraction symbol
//    for (unsigned x = 0; x < infix.length(); ++x) {
//        if (infix[x] != '-') {
//            continue;
//        }
//        if (x == 0 || infix[x - 1] == '(' || isOperator(infix[x - 1])) 
//        {
//            infix[x] = '_';
//        }
//    }
//
//    // loop thru array until there is no more data
//    for (unsigned x = 0; x < infix.length(); ++x) {
//        // place numbers (standard, decimal, & negative)
//        // numbers onto the 'postfix' string
//        if (isNumeric(infix[x])) {
//            if (postfix.length() > 0 && !isNumeric(postfix.back())) 
//            {
//                if (!std::isspace(postfix.back())) 
//                {
//                    postfix += " ";
//                }
//            }
//            postfix += infix[x];
//
//        }
//        else if (std::isspace(infix[x])) 
//        {
//            continue;
//
//        }
//        else if (isOperator(infix[x])) 
//        {
//            if (postfix.length() > 0 && !std::isspace(postfix.back())) 
//            {
//                postfix += " ";
//            }
//            // use the 'orderOfOperations' function to check equality
//            // of the math operator at the top of the stack compared to
//            // the current math operator in the infix string
//            while ((!charStack.empty()) &&
//                (precedence(charStack.top()) >= precedence(infix[x])))
//            {
//                // place the math operator from the top of the
//                // stack onto the postfix string and continue the
//                // process until complete
//                if (postfix.length() > 0 && !std::isspace(postfix.back())) 
//                {
//                    postfix += " ";
//                }
//                postfix += charStack.top();
//                charStack.pop();
//            }
//            // push the remaining math operator onto the stack
//            charStack.push(infix[x]);
//        }
//        // push outer parentheses onto stack
//        else if (infix[x] == '(') 
//        {
//            charStack.push(infix[x]);
//
//        }
//        else if (infix[x] == ')') {
//            // pop the current math operator from the stack
//            while ((!charStack.empty()) && (charStack.top() != '(')) 
//            {
//                if (postfix.length() > 0 && !std::isspace(postfix.back())) 
//                {
//                    postfix += " ";
//                }
//                // place the math operator onto the postfix string
//                postfix += charStack.top();
//                // pop the next operator from the stack and
//                // continue the process until complete
//                charStack.pop();
//            }
//
//            // pop '(' symbol off the stack
//            if (!charStack.empty()) 
//            {
//                charStack.pop();
//            }
//            else {
//                // no matching '('
//                throw std::invalid_argument{ "PARENTHESES MISMATCH" };
//            }
//        }
//        else {
//            throw std::invalid_argument{ "INVALID INPUT" };
//        }
//    }
//
//    // place any remaining math operators from the stack onto
//    // the postfix array
//    while (!charStack.empty()) {
//        if (charStack.top() == '(' || charStack.top() == ')') {
//            throw std::invalid_argument{ "PARENTHESES MISMATCH" };
//        }
//        if (postfix.length() > 0 && !std::isspace(postfix.back())) {
//            postfix += " ";
//        }
//        postfix += charStack.top();
//        charStack.pop();
//    }
//
//    // replace all '~' symbols with a minus sign
//    postfix = replaceAll(postfix, "~", "-");
//
//    return postfix;
//}// end of convertInfixToPostfix
//
//long double expressionSolver::solvePostfix(std::string& expression)
//{
//    std::stack<long double> holderStack;
//    std::string str = "";
//
//    for (int index = 0; index < expression.length(); index++)
//    {
//        if (isNumber(expression[index])) 
//        {
//            switch (expression[index])
//            {
//                case '0':
//                    //holderStack.push(0);
//                    str += '0';
//                    break;
//                case '1':
//                    //holderStack.push(1);
//                    str += '1';
//                    break;
//                case '2':
//                    //holderStack.push(2);
//                    str += '2';
//                    break;
//                case '3':
//                    //holderStack.push(3);
//                    str += '3';
//                    break;
//                case '4':
//                    //holderStack.push(4);
//                    str += '4';
//                    break;
//                case '5':
//                    //holderStack.push(5);
//                    str += '5';
//                    break;
//                case '6':
//                    //holderStack.push(6);
//                    str += '6';
//                    break;
//                case '7':
//                    //holderStack.push(7);
//                    str += '7';
//                    break;
//                case '8':
//                    //holderStack.push(8);
//                    str += '8';
//                    break;
//                case '9':
//                    //holderStack.push(9);
//                    str += '9';
//            }
//        }
//        else if (expression[index] == '.')
//        {
//            str += '.';
//        }
//        else
//        {
//            if (!str.empty())
//            {
//                holderStack.push(std::stold(str));
//                str.clear();
//            }
//
//            long double operandOne;
//            long double operandTwo;
//
//            if (expression[index] == '+')
//            { 
//                operandOne = holderStack.top();
//                holderStack.pop();
//                operandTwo = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(operandTwo + operandOne);
//            }
//            else if (expression[index] == '-')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//                operandTwo = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(operandTwo - operandOne);
//            }
//            else if (expression[index] == '*')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//                operandTwo = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(operandTwo * operandOne);
//            }
//            else if (expression[index] == '/')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//                operandTwo = holderStack.top();
//                holderStack.pop();
//                // Divide by zero: Throw exception.
//
//                holderStack.push(operandTwo / operandOne);
//            }
//            else if (expression[index] == '_')
//            {
//                if (holderStack.empty()) holderStack.push(-1);
//                else
//                {
//                    operandOne = holderStack.top();
//                    holderStack.pop();
//
//                    holderStack.push((-1) * operandOne);
//                }
//            }
//            else if (expression[index] == '^')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//                operandTwo = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(pow(operandTwo, operandOne));
//            }
//            else if (expression[index] == '!')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(sin(operandOne));
//            }
//            else if (expression[index] == '@')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(cos(operandOne));
//            }
//            else if (expression[index] == '$')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(tan(operandOne));
//            }
//            else if (expression[index] == '%')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//
//                holderStack.push(1 / tan(operandOne));
//            }
//            else if (expression[index] == '&')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//
//                // Zero or negative then throw exception.
//                holderStack.push(log(operandOne));
//            }
//            else if (expression[index] == '~')
//            {
//                operandOne = holderStack.top();
//                holderStack.pop();
//
//                // Zero or negative then throw exception.
//                holderStack.push(log10(operandOne));
//            }
//            else
//            {
//                // Throw exception.
//            }
//        }
//    }
//
//    if (holderStack.size() != 1)
//    {
//        // Throw exception.
//    }
//
//    return holderStack.top();
//}