#include "SSNSet.h"
#include <iostream>

SSNSet::SSNSet() {}

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
    int i = 0;
    while(m_set.get(i++, value))
        std::cout << value << std::endl;
    }
}
