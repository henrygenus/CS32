bool somePredicate(double x);
bool anyTrue(const double a[], int n);
int countTrue(const double a[], int n);
int firstTrue(const double a[], int n);
int positionOfMin(const double a[], int n);
bool includes(const double a1[], int n1, const double a2[], int n2);

#include <iostream>
#include <cassert>
int main()
{
    double a[] = {};
    double b[] = {2};
    double c[] = {1, 2, -3};
    double d[] = {1, -2, 3, -3};
    
    std::cerr << "Anytrue Tests: " << std::endl;
    assert ( ! anyTrue(a, sizeof(a)/sizeof(double)));
    std::cerr << "Test 1 Passed." << std::endl;
    assert ( ! anyTrue(b, sizeof(b)/sizeof(double)));
    std::cerr << "Test 2 Passed." << std::endl;
    assert (anyTrue(c, sizeof(c)/sizeof(double)));
    std::cerr << "Test 3 Passed." << std::endl;
    assert (anyTrue(d, sizeof(d)/sizeof(double)));
    std::cerr << "Test 4 Passed." << std::endl;
    std::cerr << "Anytrue Tests Passed." << std::endl << std::endl;
    
    std::cerr << "CountTrue Tests: " << std::endl;
    assert (countTrue(a, sizeof(a)/sizeof(double)) == 0);
    std::cerr << "Test 1 Passed." << std::endl;
    assert (countTrue(b, sizeof(b)/sizeof(double)) == 0);
    std::cerr << "Test 2 Passed." << std::endl;
    assert (countTrue(c, sizeof(c)/sizeof(double)) == 1);
    std::cerr << "Test 3 Passed." << std::endl;
    assert (countTrue(d, sizeof(d)/sizeof(double)) == 2);
    std::cerr << "Test 4 Passed." << std::endl;
    std::cerr << "Anytrue Tests Passed." << std::endl << std::endl;
    
    std::cerr << "FirstTrue Tests: " << std::endl;
    assert (firstTrue(a, sizeof(a)/sizeof(double)) == -1);
    std::cerr << "Test 1 Passed." << std::endl;
    assert (firstTrue(b, sizeof(b)/sizeof(double)) == -1);
    std::cerr << "Test 2 Passed." << std::endl;
    assert (firstTrue(c, sizeof(c)/sizeof(double)) == 2);
    std::cerr << "Test 3 Passed." << std::endl;
    assert (firstTrue(d, sizeof(d)/sizeof(double)) == 1);
    std::cerr << "Test 4 Passed." << std::endl;
    std::cerr << "Firsttrue Tests Passed." << std::endl << std::endl;
    
    
    std::cerr << "PositionofMin Tests: " << std::endl;
    assert (positionOfMin(a, sizeof(a)/sizeof(double)) == -1);
    std::cerr << "Test 1 Passed." << std::endl;
    assert (positionOfMin(b, sizeof(b)/sizeof(double)) == 0);
    std::cerr << "Test 2 Passed." << std::endl;
    assert (positionOfMin(c, sizeof(c)/sizeof(double)) == 2);
    std::cerr << "Test 3 Passed." << std::endl;
    assert (positionOfMin(d, sizeof(d)/sizeof(double)) == 3);
    std::cerr << "Test 4 Passed." << std::endl;
    std::cerr << "PositionofMin Tests Passed." << std::endl << std::endl;
    
    std::cerr << "Includes Tests: " << std::endl;
    assert (includes(c, sizeof(c)/sizeof(double),
                     b, sizeof(b)/sizeof(double)));
    std::cerr << "Test 1 Passed." << std::endl;
    assert (includes(b, sizeof(b)/sizeof(double),
                     a, sizeof(a)/sizeof(double)));
    std::cerr << "Test 2 Passed." << std::endl;
    assert ( ! includes(a, sizeof(a)/sizeof(double),
                        b, sizeof(b)/sizeof(double)));
    std::cerr << "Test 3 Passed." << std::endl;
    assert ( ! includes(d, sizeof(d)/sizeof(double),
                        b, sizeof(b)/sizeof(double)));
    std::cerr << "Test 4 Passed." << std::endl;
    std::cerr << "Includes Tests Passed." << std::endl << std::endl;

    std::cerr << "All Tests Passed" << std::endl;
}
