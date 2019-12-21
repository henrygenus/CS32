#include <stack>
#include <string>
#include <iostream>
#include <ctype.h>
#include <cassert>
#include "Set.h"
#include "eval.h"
using namespace std;

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool &result)
    // return 3 if char is in true & false, 2 if char is neither, 1 if other error
    // return 0 and set result if successful
{
    //attempt to create postfix expression
    bool returnValue = 0; int ret;
    if (! toPostFix(infix, postfix))
        // expression could not be converted to a valid postfix expression
        return 1;
    else if ((ret = doEval(postfix, trueValues, falseValues, returnValue)))
        //function did not perform correctly
        return ret;
    result = bool(returnValue);
    return 0;
}

// //////////////////////////////////////////////////
// ///////////////    converting     /////////////////
// //////////////////////////////////////////////////


bool toPostFix(string infix, string& postfix)
// return 1 if error
// return 0 and set postfix on success
{
    string temp_string; unsigned long temp = 0;
    stack<char> operators;
    for (int i = 0; i < infix.length(); i++)
    {
        switch(infix[i])
        {
            case '(':
                temp = temp_string.length();
                operators.push('(');
                break;
            case ')':
                while (! operators.empty() && operators.top() != '(')
                {
                    temp_string += operators.top();
                    operators.pop();
                }
                if (operators.empty())
                    return false;
                if ( ! parse(temp_string.substr(temp, temp_string.length() - temp)))
                    return false;
                operators.pop();
                break;
            case '&': case '|': case '!':
                while ( ! operators.empty() && operators.top() != '(')
                {
                    if (isHigherPriority(infix[i], operators.top()))
                        break;
                    temp_string += operators.top();
                    operators.pop();
                }
                operators.push(infix[i]);
                break;
            case ' ':
                break;
            default: //operand or invalid character
                if( ! islower(infix[i]))
                    return false;
                else
                    temp_string += infix[i];
                break;
        }
    }
    while ( ! operators.empty())
    {
        temp_string += operators.top();
        operators.pop();
    }
    postfix = temp_string;
    return parse(temp_string);
}

bool isHigherPriority(char current, char top)
{
    //presidence: ! > & > |
    switch(current)
    {
        case '!':
            return true;
        case '|':
            return false;
        default: //case '&':
            if (top == '|')
                return true;
            else //top == !
                return false;
    }
}

bool parse(string s)
    // return whether the section is a valid boolean expression
{
    if (s == "")
        return false;
    if (s.length() == 1 && isalpha(s[0]))
        return true;
    int charCount = 0;
    for (int i = 0; i < s.length(); i++)
    {
        switch(s[i])
        {
            case ' ':
                continue;
            case '!':
                if (i == 0)
                    return false;
                break;
            case '&': case '|':
                if (i < 2)
                    return false;
                charCount--;
                continue;
            default:
                charCount++;
        }
    }
    if (charCount != 1)
        return false;
    return true;
}

int doEval(string postfix, const Set& True, const Set& False, bool &returnValue)
{
    //assumed string is properly formatted
    stack<char> operands;
    char value;
    int ret;
    bool temp = false, value1 = false, value2 = false;
        //default value does not matter, these will always be initialized
    for (int i = 0; i < postfix.length(); i++)
    {
        if (islower(postfix[i]))
        {
            if ((ret = getTruthValue(postfix[i], temp, True, False)))
                return ret;
            else
                operands.push(postfix[i]);
        }
        else //ch is an operator
        {
            if ((ret = getTruthValue(operands.top(), value1, True, False)))
                return ret;
            operands.pop();
            if (postfix[i] != '!')
            {
                if ((ret = getTruthValue(operands.top(), value2, True, False)))
                    return ret;
                operands.pop();
            }
           //call operate with empty second operand for '!'
            if (operate(value1, postfix[i], value2) == true)
                True.get(0, value);
            else //operation returns false
                False.get(0, value);
            operands.push(value);
        }
    }
    return ((ret = getTruthValue(operands.top(), returnValue, True, False)));
}

int getTruthValue(char c, bool &value, const Set& True, const Set& False)
{
    if (True.contains(c) && False.contains(c))
        return 3;
    else if (True.contains(c))
    {
        value = true;
        return 0;
    }
    else if (False.contains(c))
    {
        value = false;
        return 0;
    }
    else //character is in neither set
        return 2;
}

// //////////////////////////////////////////////////
// ///////////////    operating     /////////////////
// //////////////////////////////////////////////////

bool operate(bool operand1, char op, bool operand2)
// assumes error checking has already been done & chars have been converted to bool
{
    switch(op)
    {
        case '|':
            return (operand1 || operand2);
        case '&':
            return (operand1 && operand2);
        default: //case '!':
            return ( ! operand1);
    }
}
