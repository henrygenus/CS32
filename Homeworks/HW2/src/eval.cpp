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
    int returnValue = 0;
    if (! toPostFix(infix, postfix))
        // expression could not be converted to a valid postfix expression
        return 1;
    else if (doEval(postfix, trueValues, falseValues, returnValue) == 1)
        //function did not perform correctly
        return returnValue;
    result = returnValue;
    return true;
}

// //////////////////////////////////////////////////
// ///////////////    converting     /////////////////
// //////////////////////////////////////////////////


bool toPostFix(string infix, string& postfix)
// return 1 if error
// return 0 and set postfix on success
{
    string temp_string, s; unsigned long temp = 0;
    stack<char> operators;
    int j;
    for (int i = 0; i < infix.length(); i++)
    {
        switch(infix[i])
        {
            case '(':
                temp = temp_string.length();
                operators.push('(');
                break;
            case ')':
                j = 0;
                while (operators.top() != '(')
                {
                    temp += operators.top();
                    operators.pop();
                    j++;
                }
                if (j == 0 && !islower(temp_string[postfix.length() - 1]))
                    return false;
                s = postfix.substr(temp, temp_string.length() - temp);
                if (parse(s) == false)
                    return false;
                operators.pop();
                break;
            case '&': case '|': case '!':
                while ( ! operators.empty() && operators.top() != '('
                       && !isHigherPriority(infix[i], operators.top()))
                {
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
        {
            if (top == '|')
                return true;
            else //top == !
                return false;
        }
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
                if (i == 0 || charCount != 1)
                    return false;
                charCount = 0;
                break;
            case '&': case '|':
                if (i < 2)
                    return false;
                charCount = 0;
                continue;
            default:
                charCount++;
        }
        if (charCount == 3)
            return false;
    }
    if (charCount != 0)
        return false;
    return true;
}

bool doEval(string postfix, const Set& True, const Set& False, int &error)
{
    //assumed string is properly formatted
    stack<char> operands;
    bool temp = false, char1 = false, char2 = false;
        //default value does not matter, these will always be initialized
    for (int i = 0; i < postfix.length(); i++)
    {
        if (islower(postfix[i]))
        {
            if (getTruthValue(postfix[i], temp, True, False, error) == 1)
                return false;
            operands.push(postfix[i]);
        }
        else //ch is an operator
        {
            if (getTruthValue(operands.top(), char1, True, False, error) == 1)
                return false;
            operands.pop();
            if (postfix[i] != '!')
            {
                if (getTruthValue(operands.top(), char2, True, False, error) == 1)
                    return false;
                operands.pop();
            }
           //call operate with empty second operand for '!'
            char value;
            if (operate(char1, postfix[i], char2))
                True.get(0, value);
            else //operation returns false
                False.get(0, value);
            operands.push(value);
        }
    }
    return True.contains(operands.top());
}

int getTruthValue(char c, bool &operand, const Set& trueValues,
                 const Set& falseValues, int &returnValue)
{
    if (trueValues.contains(c) && falseValues.contains(c))
    {
        returnValue = 3;
        return 1;
    }
    
    else if (trueValues.contains(c))
    {
        operand = true;
        return 0;
    }
    
    else if (falseValues.contains(c))
    {
        operand = false;
        return 0;
    }
    
    else //character is in neither set
    {
        returnValue = 2;
        return 1;
    }

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

