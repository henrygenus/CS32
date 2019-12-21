#include "SSNSet.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    SSNSet s;
    assert(s.size() == 0);
    assert(s.add(1001) && s.size() == 1);
    assert(! s.add(1001) && s.size() == 1);
    assert(s.add(01) && s.size() == 2);
    SSNSet s2 = s;
    assert(s.size() == s2.size());
    cout << "All SSN Tests Passed" << endl;
}

