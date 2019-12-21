bool somePredicate(double x)
{
    return x < 0;
}

// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
    if (n <= 0)
        return false;
    else
        return (somePredicate(*a) ? true : anyTrue(a+1, n-1));
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if (n <= 0)
        return 0;
    else
    {
        int prev = (somePredicate(*a) ? 1 : 0);
        return (prev + countTrue(a+1, n-1));
    }
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if (n <= 0)
        return -1;
    else if (somePredicate(*a))
        return 0;
    else
    {
        //in each iteration, add 1 to value
        int first = 1 + firstTrue(a+1, n-1);
        return (first == 0 ? -1 : first);
    }
}



// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1)
        return 0;
    else
    {
        int min = 1 + positionOfMin(a+1, n-1);
        return (min == 0 ? -1 :
                *a <= a[min] ? 0 : min);
    }
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise return false.
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 > n1)
        return false;
    else if (n2 <= 0)
        return true;
    else if (n1 <= 0)
        return false;
    else
    {
        if (a1[0] == a2[0])
            return (includes(a1+1, n1-1, a2+1, n2-1));
        else
            return (includes(a1+1, n1-1, a2, n2));
    }
}
