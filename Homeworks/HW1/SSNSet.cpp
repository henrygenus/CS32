#include "SSNSet.h"
#include <iostream>

SSNSet::SSNSet(){}

bool SSNSet::add(unsigned long ssn)
{
    return (m_set.insert(ssn));
}

int SSNSet::size() const
{
    return m_set.size();
}

void SSNSet::print() const
{
    unsigned long value;
    for (int i = 0; i < m_set.size(); i++)
    {
        if (! m_set.get(i, value)) break;
        std::cout << value << std::endl;
    }
}
