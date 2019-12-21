#include <stack>
#include <string>
#include <iostream>
#include <ctype.h>
#include <cassert>
#include "Set.h"
using namespace std;

int evaluate(string infix, const Set& trueValues,
             const Set& falseValues, string& postfix, bool& result);
bool toPostFix(string infix, string& postfix);
bool parse(string s);
bool isHigherPriority(char current, char top);
bool isTrue(string postfix, const Set& trueValues,
            const Set& falseValues, int &returnValue);
bool setOperator(char c, bool &operand, const Set& trueValues,
                 const Set& falseValues, int &returnValue);
bool operate(bool operand1, char op, bool operand2);


int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool &result)
{
    //attempt to create postfix expression
    if (! toPostFix(infix, postfix))
        return 1;
    else
    {
        int returnValue = 0;
        bool r = isTrue(postfix, trueValues, falseValues, returnValue);
        if (returnValue != 0)
            //function did not perform correctly
            return returnValue;
        else
        {
            result = r;
            return 0;
        }
    }
}

// //////////////////////////////////////////////////
// ///////////////    converting     /////////////////
// //////////////////////////////////////////////////


bool toPostFix(string infix, string& postfix)
{
    postfix = ""; unsigned long temp = 0;
    stack<char> operators;
    for (int i = 0; i < infix.length(); i++)
    {
        switch(infix[i]) {
            case '(':
            {
                temp = postfix.length();
                operators.push('(');
                continue;
            }
            case ')':
            {
                int j = 0;
                while (operators.top() != '(')
                {
                    postfix += operators.top();
                    operators.pop();
                    j++;
                }
                if (j == 0 && !islower(postfix[postfix.length() - 1]))
                {
                    //cerr << "Error: Empty Parentheses." << endl;
                    return false;
                }
                string s = postfix.substr(temp, postfix.length() - temp);
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
                    postfix += operators.top();
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
                    postfix += infix[i];
                continue;
            }
        }
    }
    while ( ! operators.empty())
    {
        postfix += operators.top();
        operators.pop();
    }
    return parse(postfix);
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
            else //top == |
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

bool isTrue(string postfix, const Set& trueValues, const Set& falseValues, int &returnValue)
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

int main()
{
    string trueChars  = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);
    
    string trueChar  = "aeiou";
    string falseChar = "bgkp";
    Set True;
    Set False;
    for (int k = 0; k < trueChar.size(); k++)
        True.insert(trueChar[k]);
    for (int k = 0; k < falseChar.size(); k++)
        False.insert(falseChar[k]);
    
    string pf;
    bool answer;
    int i = 17;
    for (/*i = 1*/; i <= 21; i++)
    {
        cerr << "Test " << i << "." << endl;
        switch(i)
        {
            case 1:
                assert(evaluate("w| f", trues, falses, pf, answer) == 0
                       &&  pf == "wf|" &&  answer);
                break;
            case 2:
                assert(evaluate("y|", trues, falses, pf, answer) == 1);
                break;
            case 3:
                assert(evaluate("n t", trues, falses, pf, answer) == 1);
                break;
            case 4:
                assert(evaluate("nt", trues, falses, pf, answer) == 1);
                break;
            case 5:
                assert(evaluate("()", trues, falses, pf, answer) == 1);
                break;
            case 6:
                assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
                break;
            case 7:
                assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
                break;
            case 8:
                assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
                break;
            case 9:
                assert(evaluate("", trues, falses, pf, answer) == 1);
                break;
            case 10:
                assert(evaluate("f  |  !f & (t&n) ", trues, falses,
                                pf, answer) == 0);// &&
                assert(pf == "ff!tn&&|"  &&  !answer);
                break;
            case 11:
                assert(evaluate(" x  ", trues, falses, pf, answer) == 0
                       &&  pf == "x"  &&  !answer);
                break;
            case 12:
                trues.insert('x');
                assert(evaluate("((x))", trues, falses, pf, answer) == 3);
                break;
            case 13:
                falses.erase('x');
                assert(evaluate("((x))", trues, falses, pf, answer) == 0
                       &&  pf == "x"  &&  answer);
                break;
            case 14:
                trues.erase('w');
                assert(evaluate("w| f", trues, falses, pf, answer) == 2);
                break;
            case 15:
                falses.insert('w');
                assert(evaluate("w| f", trues, falses, pf, answer) == 0
                       &&  pf == "wf|" &&  !answer);
                break;
            case 16:
                break;
                assert(evaluate("e&!(b|o&i|k)|!!!(p&e&g)", True, False, pf, answer) == 0 && pf == "eboi&|k|!&pe&g&!!!|"  && answer);
                break;
            case 17:
                break;
                assert(evaluate("(i&((b|i)&(i)))", True, False, pf, answer) == 0 && pf == "ibi|i&&"  && answer);
                break;
            case 18:
                assert(evaluate("(o|k)u", trues, falses, pf, answer) == 1);
                break;
            case 19:
                assert(evaluate("e!i", trues, falses, pf, answer) == 1);
                break;
            case 20:
                assert(evaluate("g)", trues, falses, pf, answer) == 1);
                break;
            case 21:
                assert(evaluate("(a|b))&((e|g)", trues, falses, pf, answer) == 1);
                break;
        }
    }
    cout << "Passed all tests" << endl;
}
