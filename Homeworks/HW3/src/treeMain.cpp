#include <iostream>
#include <cassert>
using namespace std;

int countIncludes(const double a1[], int n1, const double a2[], int n2);
void order(double a[], int n);

int main()
{
    double a1[] = { 10, 50, 40, 20, 50, 40, 30 };
    double a2[] = { 10, 20, 40 };
    double a3[] = { 10, 40, 30 };
    double a4[] = { 20, 10, 40 };
    double a5[] = { 50, 40, 30 };
    double a6[] = { };
    double a7[] = { 50, 50, 20, 10 };
    double a8[] = { 10, 50, 20, 50 };
    
    cerr << "Tests:" << endl << endl;
    
    cerr << "countInclude tests:" << endl;
    assert(countIncludes(a1, 7, a1, 7) == 1);
    cerr << "Test 1 Passed." << endl;
    assert(countIncludes(a1, 7, a2, 3) == 1);
    cerr << "Test 2 Passed." << endl;
    assert(countIncludes(a1, 7, a3, 3) == 2);
    cerr << "Test 3 Passed." << endl;
    assert(countIncludes(a1, 7, a4, 3) == 0);
    cerr << "Test 4 Passed." << endl;
    assert(countIncludes(a1, 7, a5, 3) == 3);
    cerr << "Test 5 Passed." << endl;
    assert(countIncludes(a1, 7, a6, 0) == 1);
    cerr << "Test 6 Passed." << endl;
    assert(countIncludes(a6, 0, a6, 0) == 1);
    cerr << "Test 7 Passed." << endl;
    assert(countIncludes(a6, 0, a1, 7) == 0);
    cerr << "Test 8 Passed." << endl;
    cerr << "countIncludes Tests Passed." << endl << endl;
    
    //does this work if the set is weighted toward the heavy/light end?
    cerr << "order Tests:" << endl;
    order(a2, 3);
    order(a4, 3);
    assert(std::equal(a2, a2 + sizeof a2 / sizeof *a2, a4));
    cerr << "Test 1 Passed." << endl;
    order(a8, 4);
    cerr << "Test 2 Passed." << endl;
    assert(std::equal(a7, a7 + sizeof a7 / sizeof *a7, a8));
    cerr << "Test 3 Passed." << endl;
    order(a6, 0);
    cerr << "Test 4 Passed." << endl;
    
    double a9[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    order(a9, 15);
    for (int i = 0; i < 15; i++)
        cout << a9[i] << " ";
    cout << endl;
    
    cerr << "order Tests Passed." << endl << endl;
    
    cerr << "All Tests Passed." << endl;
}
