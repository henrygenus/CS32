#ifndef Set_h
#define Set_h

#include <string>
//using ItemType = std::string;
using ItemType = int;

class Set
{
public:
    //utility functions
    Set();
    Set(const Set& old);
    ~Set();
    //mutators
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    void swap(Set& other);
    Set& operator=(const Set& rhs);
    //const functions
    bool empty() const;
    int size() const;
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void dump() const;
private:
    struct Node
    {
        Node* next;
        Node* prev;
        ItemType data;
    };
    Node* m_head;
    Node* m_tail;
    int m_size;
};

void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);

#endif /* Set_h */
