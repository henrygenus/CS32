#include "Set.h"
#include <iostream>

//////////////////////////////////////////
///////////     utilities      ///////////
//////////////////////////////////////////
Set::Set() 
: m_head(nullptr), m_tail(nullptr), m_size(0) {}

Set::Set(const Set& old)
{
    //make an empty set
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;

    if (old.m_size == 0) //if the set is to be empty
        return;
   
    //set temp pointer
    Node* temp = old.m_head;
    
    //insert nodes one at a time
    for (int i = 0; i < old.size(); i++)
    {
        insert(temp->data);
        temp = temp->next;
    }
    
}

Set::~Set()
{
    if (m_size != 0)
    {
        while (m_head != m_tail)
        {
            //increment temp & delete previous
            m_head = m_head->next;
            delete m_head->prev;
        }
        delete m_tail;
    }
}

//////////////////////////////////////////
///////////      mutators      ///////////
//////////////////////////////////////////
bool Set::insert(const ItemType& value) //add data to sorted location
{
    if (contains(value))
        return false;
    else if (m_size == 0)
    {
        //set single node
        m_tail = new Node;
        m_head = m_tail;
        m_tail->data = value;
        m_tail->next = nullptr;
        m_tail->prev = nullptr;
        m_size++;
        return true;
    }
    else //if m_size >= 1
    {
        //set pointer to first element larger than that to be inputted
        Node* temp = m_head;
        while (temp != nullptr && temp->data < value)
            temp = temp->next;
        
        //if the element is last, put it after m_tail
        if (temp == nullptr)
        {
            //set pointers
            m_tail->next = new Node;
            m_tail->next->next = nullptr;
            m_tail->next->prev = m_tail;
            m_tail = m_tail->next;
            
            //set value
            m_tail->data = value;
            
        }
        //otherwise, we must add the element before temp
        else
        {
            //set pointers
            Node* prevPtr = temp->prev;
            temp->prev = new Node;
            temp->prev->prev = prevPtr;
            temp->prev->next = temp;
            temp = temp->prev;
         
            //if this would be the first element, we do not want to follow NULL
            if (prevPtr != nullptr)
                prevPtr->next = temp;
            
            //set value
            temp->data = value;
        }

        //move head to beginning
        while (m_head->prev != nullptr)
            m_head = m_head->prev;
        while (m_tail->next != nullptr)
            m_tail = m_tail->next;


        //if we added an element, increment size & return true
        m_size++;
        return true;
    }
}

bool Set::erase(const ItemType& value) //delete data member, if found
{
    if (! contains(value) || m_size == 0)
        return false;
    else
    {
        Node* temp = m_head;
        if (m_size == 1)
        {
            delete temp;
            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
            return true;
        }
        else //size > 1
        {
            //set temporary pointer equal to object with VALUE
                       while(temp->data != value)
                temp = temp->next;
       
            //set pointers
            if (temp != m_head)
                temp->prev->next = temp->next;
            else //temp == head
                m_head = m_head->next;
            if (temp != m_tail)
                temp->next->prev = temp->prev;
            else //temp == tail
                m_tail = m_tail->prev;
            delete temp;
            m_size--;
            return true;
        }
    }
}

void Set::swap(Set& other)
{
    //swap heads
    Node* tempHead = other.m_head;
    other.m_head = m_head;
    m_head = tempHead;
    
    //swap tails
    Node* tempTail = other.m_tail;
    other.m_tail = m_tail;
    m_tail = tempTail;
    
    //swap sizes
    int temp = m_size;
    m_size = other.m_size;
    other.m_size = temp;
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

//////////////////////////////////////////
//////////    const functions  ///////////
//////////////////////////////////////////
bool Set::empty() const //return whether set is empty
{ return (m_size == 0 ? true : false); }

int Set::size() const //return size of list
{ return m_size; }

bool Set::contains(const ItemType& value) const //find a data member
{
    Node* temp= m_head;
    while(temp != nullptr)
    {
        if (temp->data == value)
            return true;
        temp = temp->next;
    }
    return false;
}

bool Set::get(int pos, ItemType& value) const //return data larger than pos items
{
    if (pos + 1 > m_size || pos < 0 || m_size == 0)
        return false;
    else
    {
        Node* temp = m_head;
        for (int i = 0; i < pos; i++)
            temp = temp->next;
        value = temp->data;
        //std::cerr << "value = " << value << std::endl;
        return true;
    }
}

void Set::dump() const
{
    std::cerr << "Printing Set.  m_size: " << m_size << std::endl
        << std::string(10,'-') << std::endl;
    for (Node* temp = m_head; temp != nullptr; temp = temp->next)
        std::cerr << temp->data << std::endl;
    std::cerr << std::string(10,'-') << std::endl;
    std::cerr << "m_head: " << m_head
    << "\nm_tail: " << m_tail << std::endl << std::endl;
}

//////////////////////////////////////////
//////////Non-Member functions///////////
//////////////////////////////////////////

void unite(const Set& s1, const Set& s2, Set& result)
{
    result = s1;
    ItemType value;
    for (int i = 0; i < s2.size(); i++)
    {
        s2.get(i, value);
        result.insert(value);
    }
}


void subtract(const Set& s1, const Set& s2, Set& result)
{
    result = s1;
    ItemType value;
    for (int i = 0; i < s2.size(); i++)
    {
        s2.get(i, value);
        result.erase(value);
    }
}
