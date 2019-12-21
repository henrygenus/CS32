#include "newSet.h"

Set::Set(int max)        // Create an empty set (i.e., one with no items).
: m_max(max)
{
    m_num = 0;
    m_items = new ItemType[max];
}

Set::Set(const Set& other)
{
    m_num = other.m_num;
    m_max = other.m_max;
    m_items = new ItemType[m_max];
    for (int i = 0; i < m_num; i++)
        m_items[i] = other.m_items[i];
}

Set::~Set()
{
    if (m_max != 0)
        delete m_items;
}
bool Set::empty()  const// Return true if the set is empty, otherwise false.
{
    return (m_num == 0 ?  true : false);
}

int Set::size() const// Return the number of items in the set.
{
    return m_num;
}

bool Set::insert(const ItemType& value)
// Insert value into the set if it is not already present.  Return
// true if the value was actually inserted.  Leave the set unchanged
// and return false if the value was not inserted (perhaps because it
// was already in the set or because the set has a fixed capacity and
// is full).
{
    //check if items are maxed
    if (m_num == m_max) return false;
    //check if item already exists
    if (contains(value)) return false;
    
    //assign
    ItemType* ptr = m_items;
    int i, j;
    for (i = 0; i < m_num; i++)
        if (*ptr <= m_items[i])
            break;
    for (j = m_num; j > i; j--)
        m_items[j] = m_items[j-1];
    m_items[i] = value;
    m_num++;
    return true;
}

bool Set::erase(const ItemType& value)
// Remove the value from the set if present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.
{
    for (int k = 0; k < m_num; k++)
    {
        if (m_items[k] == value)
        {
            for (int l = k; l < m_num - 1; l++)
                m_items[l] = m_items[l+1];
            m_num--;
            return true;
        }
    }
    return false;
}

bool Set::contains(const ItemType& value) const
// Return true if the value is in the set, otherwise false.
{
    for (int l = 0; l < m_num; l++)
    {
        if (m_items[l] == value)
            return true;
        if (m_items[l] < value)
            break;
    }
    return false;
}

bool Set::get(int i, ItemType& value) const
// If 0 <= i < size(), copy into value the item in the set that is
// strictly greater than exactly i items in the set and return true.
// Otherwise, leave value unchanged and return false.
{
    if (i >= m_num)
        return false;
    value = m_items[i];
    return true;
}

void Set::swap(Set& other)
// Exchange the contents of this set with the other one.
{
    //swap arrays
    ItemType* temp;
    temp = m_items;
    m_items = other.m_items;
    m_items = temp;
    
    //swap item numbers
    int tempInt = other.m_num;
    other.m_num = m_num;
    m_num = tempInt;
    
    //swap max array sizes
    int tempMax = other.m_max;
    m_max = other.m_max;
    m_max = tempMax;
}

Set& Set::operator=(const Set& rhs)
{
    if (this != &rhs)
    {
        Set temp(rhs);
        swap(temp);
    }
    return *this;
}

