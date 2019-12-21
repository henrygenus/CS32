#ifndef eval_h
#define eval_h

#include <string>
#include "Set.h"
using namespace std;

int evaluate(string in, const Set& T, const Set& F, string& post, bool& result);
    // return 3 if char is in true & false, 2 if char is neither, 1 if other error
    // return 0 and set result if successful
bool toPostFix(string infix, string& postfix);
    // return 1 if error
    // return 0 and set postfix on success
bool parse(string s);
bool isHigherPriority(char current, char top);
int doEval(string postfix, const Set& True, const Set& False, bool &returnValue);
int getTruthValue(char c, bool &operand, const Set& True, const Set& False);
bool operate(bool operand1, char op, bool operand2);

#endif /* eval_h */
