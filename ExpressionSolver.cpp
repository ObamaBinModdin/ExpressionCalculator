#include "ExpressionSolver.h"

/*
* An expression of type string is passed in. The expression is checked if it is empty or does
* not contain a number. If either is true then an invalid_argument exception is thrown.
* All trigonometric are replaced with single characters for processing.
* Then the expression is converted to postfix before being solved.
* Returns the answer of type long double.
*/
long double expressionSolver::solve(std::string& expression)
{
    // Throws invalid_argument exception if expression is empty.
    if (expression.empty()) throw std::invalid_argument("ERROR: NO INPUT\n");

    // Throws invalid_argument exception if no numeric digits are in expression.
    if (std::find_if(expression.begin(), expression.end(), ::isdigit) == expression.end())
    {
        throw std::invalid_argument("ERROR: NO NUMBERS WERE INPUTTED\n");
    }

    // All trigonometric functions are converted to a single character for processing later.
    replaceTrig(expression);

    // Expression is converted from infix to postfix.
    expression = convertInfixToPostfix(expression);

    // Returns the answer after evaluating the postfix expression.
    return evaluatePostfix(expression);
}// End of solve().

/*
* An infix expression of type string is passed in as an argument.
* Removes all whitespace from infix. Checks if the last character is an equal sign. This
* is optional to have but there can only be one and it has to be the last character. If invalid equal sign
* then throw an exception. Remove all '--' since they cancel each other out. Distinquish negative and negation operations.
* Finally, convert to postfix using the shunting algorithm. Returns the postfix expression of type string.
*/
std::string expressionSolver::convertInfixToPostfix(std::string infix) {
    // Declare function variables
    std::string postfix;
    std::stack<char> charStack;

    // Remove all whitespace from the string
    infix.erase(std::remove_if(infix.begin(), infix.end(), [](char c)
        {
            return std::isspace(static_cast<unsigned char>(c));
        }), infix.end());

    // Remove equal sign if present at the end. Throws invalid_argument if multiple equal signs.
    if (infix[infix.length() - 1] == '=')
    {
        infix.pop_back();

        if (infix.find('=') != std::string::npos)
        {
            throw std::invalid_argument{ "ERROR: INVALID '=' PLACEMENT\n" };
        }
    }

    // Negate equations marked with '--'.
    infix = replaceAll(infix, "--", "+");


    // Convert negative numbers to have the ~ symbol.
    // This is done so we can distinguish negative numbers and the subtraction symbol.
    for (unsigned x = 0; x < infix.length(); ++x)
    {
        if (infix[x] != '-')
        {
            continue;
        }
        if (x == 0 || infix[x - 1] == '(' || isMathOperator(infix[x - 1]))
        {
            infix[x] = '~';
            flipOperators(x, infix);
        }
    }


    // Loop thru array until there is no more data in infix.
    for (unsigned x = 0; x < infix.length(); ++x)
    {
        // Place numbers (standard, decimal, & negative) numbers onto the 'postfix' string.
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
        // Skips spaces if any should still be present.
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
            // Use the 'orderOfOperations' function to check equality of the math operator at the top of
            // the stack compared to the current math operator in the infix string.
            while ((!charStack.empty()) &&
                (orderOfOperations(charStack.top()) >= orderOfOperations(infix[x])))
            {
                // Place the math operator from the top of the stack onto the postfix string and continue the
                // process until complete.
                if (postfix.length() > 0 && !std::isspace(postfix.back()))
                {
                    postfix += " ";
                }
                postfix += charStack.top();
                charStack.pop();
            }
            // Push the remaining math operator onto the stack.
            charStack.push(infix[x]);
        }
        // Push outer parentheses onto stack.
        else if (infix[x] == '(')
        {
            charStack.push(infix[x]);
        }
        else if (infix[x] == ')')
        {
            // Pop the current math operator from the stack.
            while ((!charStack.empty()) && (charStack.top() != '('))
            {
                if (postfix.length() > 0 && !std::isspace(postfix.back()))
                {
                    postfix += " ";
                }
                // Place the math operator onto the postfix string.
                postfix += charStack.top();
                // Pop the next operator from the stack and continue the process until complete.
                charStack.pop();
            }

            // Pop '(' symbol off the stack.
            if (!charStack.empty())
            {
                charStack.pop();
            }
            else
            {
                // No matching '('
                throw std::invalid_argument{ "ERROR: PARENTHESES MISMATCH\n" };
            }
        }
        else
        {
            throw std::invalid_argument{ "ERROR: INVALID INPUT\n" };
        }
    }

    // Place any remaining math operators from the stack onto the postfix array.
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

    // Replace all '~' symbols with a minus sign.
    postfix = replaceAll(postfix, "~", "-");

    return postfix;
}// End of convertInfixToPostfix().

/*
* Takes in a token of type char and determines if the char is an operator.
* Returns true if operator. False if not.
*/
bool expressionSolver::isMathOperator(char token)
{
    // This function checks if operand is a math operator.
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
}// End of isMathOperator().

/*
* Takes in a token of type char. Returns an int based on the priority of the token.
*/
int expressionSolver::orderOfOperations(char token)
{
    int priority = 0;
    switch (std::tolower(token))
    {
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
}// End of orderOfOperations().

/*
* Takes in a postfix of type string then evaluates the expression to return a single
* long double.
*/
long double expressionSolver::evaluatePostfix(const std::string& postfix)
{
    // Declare function variables.
    long double answer = 0;
    std::stack<long double> doubleStack;

    // Split string into tokens to isolate multi digit, negative and decimal
    // numbers, aswell as single digit numbers and math operators.
    auto tokens = split(postfix);

    // Loop thru array until there is no more data.
    for (unsigned x = 0; x < tokens.size(); ++x) {
        auto token = tokens[x];

        // Push numbers & negative numbers onto the stack.
        if (isNumeric(token)) {
            doubleStack.push(std::atof(token.c_str()));
        }
        // If expression is a math operator, pop numbers from stack
        // & send the popped numbers to the 'calculate' function.
        else if (isMathOperator(token[0]) && (!doubleStack.empty()))
        {
            long double value1 = 0;
            long double value2 = 0;
            char mathOperator = static_cast<unsigned char>(std::tolower(token[0]));

            // If expression is ln, log, cot, sin, cos,
            // or tan operation only pop stack once
            if (mathOperator == 's' || mathOperator == 'c' || mathOperator == 't' || mathOperator == 'v'
                || mathOperator == 'l' || mathOperator == 'n')
            {
                value2 = 0;
                value1 = doubleStack.top();
                doubleStack.pop();
                answer = calculate(mathOperator, value1, value2);
                doubleStack.push(answer);

            }
            else if (doubleStack.size() > 1)
            {
                value2 = doubleStack.top();
                doubleStack.pop();
                value1 = doubleStack.top();
                doubleStack.pop();
                answer = calculate(mathOperator, value1, value2);
                doubleStack.push(answer);
            }
        }
        else
        {
            // This should never execute, & if it does, something went really wrong
            throw std::invalid_argument{ "ERROR: INVALID POSTFIX STRING\n" };
        }
    }
    // pop the final answer from the stack, and return to main
    if (!doubleStack.empty()) {
        answer = doubleStack.top();
    }
    return answer;
}// End of evaluatePostfix().

/*
* An operator of type char and two values of type long double are passed in.
* Solves one operation based on the operation passed in.
* Returns the answer of type long double.
*/
long double expressionSolver::calculate(char mathOperator, long double value1, long double value2)
{
    long double ans = 0;
    switch (std::tolower(mathOperator))
    {
        // Addition
    case '+':
        ans = value1 + value2;
        break;
        // Subtraction
    case '-':
        ans = value1 - value2;
        break;
        // Multiplication
    case '*':
        ans = value1 * value2;
        break;
        // Division
    case '/':
        // Throw runtime_error if dividing by zero.
        if (value2 == 0)
        {
            throw std::runtime_error("ERROR: ATTEMPTED TO DIVIDE BY ZERO\n");
        }

        ans = value1 / value2;
        break;
        // Modular
    case '%':
        ans = ((int)value1 % (int)value2) + std::modf(value1, &value2);
        break;
        // Exponent
    case '^':
        ans = std::pow(value1, value2);
        break;
        // Cosine
    case 'c':
        ans = std::cos(value1);
        break;
        // Sine
    case 's':
        ans = std::sin(value1);
        break;
        // Tanget
    case 't':
        ans = std::tan(value1);
        break;
        // Cotanget
    case 'v':
        // Throw runtime_error if trying to divide by zero.
        if (std::tan(value1) == 0) throw std::runtime_error("ERROR: COT WAS UNDEFINED\n");
        ans = 1 / std::tan(value1);
        break;
        // Natural Log
    case 'l':
        if (value1 < 0) throw std::runtime_error("ERROR: ATTEMPTED TO LOG A NEGATIVE\n");
        if (value1 == 0) throw std::runtime_error("ERROR: ATTEMPTED TO LOG ZERO\n");
        ans = std::log(value1);
        break;
        // Log base 10
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
}// End of calculate ().

/*
* Split string into tokens to isolate multi digit, negative and decimal
* numbers, aswell as single digit numbers and math operators.
*/
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
}// End of split().

/*
* Replaces a character or string to a different character or string.
* Returns string.
*/
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
    while (std::string::npos != (findPos = source.find(oldValue, lastPos)))
    {
        newString.append(source, lastPos, findPos - lastPos);
        newString += newValue;
        lastPos = findPos + oldValue.length();
    }
    newString += source.substr(lastPos);
    return newString;
}// End of replaceAll().

/*
* Takes in a value of char and determines if it is a number. If not then return false.
* Otherwise return true.
*/
bool expressionSolver::isNumeric(char value)
{
    return std::isdigit(value) || value == '.' || value == '~';
}// End of isNumeric().

bool expressionSolver::isNumeric(std::string value)
{
    for (unsigned index = 0; index < value.length(); ++index)
    {
        if (index == 0 && value[index] == '-' && value.length() > 1)
        {
            continue;
        }
        if (!isNumeric(value[index]))
        {
            return false;
        }
    }
    return true;
}

/*
* Replaces all functions to a one digit character.
*/
void expressionSolver::replaceTrig(std::string& expression)
{
    expression = std::regex_replace(expression, std::regex("sin"), "s");

    expression = std::regex_replace(expression, std::regex("cos"), "c");

    expression = std::regex_replace(expression, std::regex("tan"), "t");

    expression = std::regex_replace(expression, std::regex("cot"), "v");

    expression = std::regex_replace(expression, std::regex("ln"), "l");

    expression = std::regex_replace(expression, std::regex("log"), "n");
} // End of replaceTrig().

/*
* If a pair of parentheses follows a negation then flip all operands in the paratheses.
*/
void expressionSolver::flipOperators(int start, std::string& expression)
{
    std::stack<char> charStack;

    for (size_t index = start + 1; index < expression.length(); index++)
    {
        if (charStack.empty()) break;
        else if (expression[index] == '(') charStack.push('(');
        else if (expression[index] == '-') expression[index] = '+';
        else if (expression[index] == '+') expression[index] = '-';
        else if (expression[index] == ')') charStack.pop();

        if (charStack.empty()) break;
    }
}// End of flipOperands().