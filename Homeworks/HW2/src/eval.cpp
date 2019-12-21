#include <stack>
#include <string>
#include <iostream>
#include <ctype.h>
#include <cassert>
#include "Set.h"
#include "eval.h"
using namespace std;

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool &result)
    // return 1 if error, 0 if evaluated
{
    //attempt to create postfix expression
    int returnValue = 0;
    if (! toPostFix(infix, postfix))
        return 1;
    else
    {
        if (doEval(postfix, trueValues, falseValues, returnValue) == 1)
                //function did not perform correctly
        {
            return returnValue;
        }
        else
        {
            result = returnValue;
            return true;
        }
    }
}

// //////////////////////////////////////////////////
// ///////////////    converting     /////////////////
// //////////////////////////////////////////////////


bool toPostFix(string infix, string& postfix)
{
    string temp_string = ""; unsigned long temp = 0;
    stack<char> operators;
    for (int i = 0; i < infix.length(); i++)
    {
        switch(infix[i]) {
            case '(':
            {
                temp = temp_string.length();
                operators.push('(');
                continue;
            }
            case ')':
            {
                int j = 0;
                while (operators.top() != '(')
                {
                    temp += operators.top();
                    operators.pop();
                    j++;
                }
                if (j == 0 && !islower(temp_string[postfix.length() - 1]))
                {
                    //cerr << "Error: Empty Parentheses." << endl;
                    return false;
                }
                string s = postfix.substr(temp, temp_string.length() - temp);
                if (parse(s) == false)
                {
                    //cerr << "Error: Invalid Substring Within Parentheses." << endl;
                    return false;
                }
                operators.pop();
                continue;
            }
            case '&': case '|': case '!':
            {
                while ( ! operators.empty() && operators.top() != '('
                       && !isHigherPriority(infix[i], operators.top()))
                {
                    temp_string += operators.top();
                    operators.pop();
                }
                operators.push(infix[i]);
                continue;
            }
            case ' ':
                continue;
            default: //operand or invalid character
            {
                if( ! islower(infix[i]))
                {
                    //cerr << "Error: Invalid Character." << endl;
                    return false;
                }
                else
                    temp_string += infix[i];
                continue;
            }
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

bool parse(string s) //if a subsection is true or false, replace it
{
    if (s == "")
        return false;
    if (s.length() == 1 && isalpha(s[0]))
        return true;
    int charCount = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '!')
        {
            if (i == 0 || !islower(s[i-1]))
                return false;
            charCount = 0;
            s.erase(i, 1);
            continue;
        }
        else if (s[i] == '&' || s[i] == '|')
            {
                if (i < 2 || !islower(s[i-1]) || !islower(s[i-2]))
                    return false;
                charCount = 0;
                s.erase(i-1, 2);
                continue;
            }
        else if (s[i] == ' ')
            continue;
        else
            charCount++;
        if (charCount == 3)
            return false;
    }
    if (charCount != 0)
        return false;
    return true;
}

// //////////////////////////////////////////////////
// ///////////////    operating     /////////////////
// //////////////////////////////////////////////////

bool doEval(string postfix, const Set& trueValues, const Set& falseValues, int &returnValue)
{
    //assumed string is properly formatted
    stack<char> operands;
    for (int i = 0; i < postfix.length(); i++)
    {
        if (islower(postfix[i]))
        {
            bool temp = true;
            if ( ! setOperator(postfix[i], temp, trueValues,
                               falseValues, returnValue))
                return false;
            operands.push(postfix[i]);
        }
        else //ch is an operator
        {
            //default value does not matter, these will always be initialized
            bool operand1 = false, operand2 = false;
            if ( ! setOperator(operands.top(), operand1, trueValues, falseValues, returnValue))
                return false;
            operands.pop();
            if (postfix[i] != '!')
            {
                if ( ! setOperator(operands.top(), operand2, trueValues,
                            falseValues, returnValue))
                    return false;
                operands.pop();
            }
           //call operate with empty second operand for '!'
            char value;
            if (operate(operand1, postfix[i], operand2))
                trueValues.get(0, value);
            else //operation returns false
                falseValues.get(0, value);
            operands.push(value);
        }
    }
    return trueValues.contains(operands.top());
}

bool setOperator(char c, bool &operand, const Set& trueValues,
                 const Set& falseValues, int &returnValue)
{
    if (trueValues.contains(c) && falseValues.contains(c))
    {
        //cerr << "Value Both True and False" << endl;
        returnValue = 3;
        return false;
    }
    
    else if (trueValues.contains(c))
    {
        operand = true;
        return true;
    }
    
    else if (falseValues.contains(c))
    {
        operand = false;
        return true;
    }
    
    else //character is in neither set
    {
        //cerr << "Value Neither True Nor False" << endl;
        returnValue = 2;
        return false;
    }

}

bool operate(bool operand1, char op, bool operand2)
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

