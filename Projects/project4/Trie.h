#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <map>

template<typename ValueType>
struct node
{
    int m_nChildren;
    std::vector<ValueType> m_values;
    std::vector<node*> m_children;
    std::string m_base;
};

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    node<ValueType>* root;
        //pointer to the root of the tree
    void doClear(node<ValueType>* N);
        //deletes entire tree
    void addValue(node<ValueType>* next, std::string key, ValueType value);
        //adds a value to its correct place on the tree
    node<ValueType>* addNode();
        //adds and returns an empty node
    void addNode(node<ValueType>* next, char base);
        //adds a node with the base needed to continue down the tree
    void doFind(node<ValueType>* next, std::vector<ValueType>& v,  std::string key, bool exactMatchOnly) const;
        //follows the key to return a concatonation of relevant vectors
};

#endif // TRIE_INCLUDED

template<typename ValueType>
Trie<ValueType>::Trie()
: root(addNode())
{}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
    doClear(root);
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
    doClear(root);
    delete root;
    root = addNode();
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
    addValue(root, key, value);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
    std::vector<ValueType> v;
    doFind(root, v, key, exactMatchOnly);
    return v;
}

template<typename ValueType>
void Trie<ValueType>::doClear(node<ValueType>* N)
{
    if (N == nullptr)
        return;
    for (int i = 0; i < N->m_nChildren; i++)
        doClear(N->m_children[i]);
    for (int i = 0; i < N->m_nChildren; i++)
        delete (N->m_children[i]);
}

template<typename ValueType>
void Trie<ValueType>::addValue(node<ValueType>* next, std::string key, ValueType value)
{
    if (key == "")
    {
        next->m_values.push_back(value);
        return;
    }
    
    if (next->m_nChildren != 0)
    {
        for (int i = 0; i < next->m_nChildren; i++)
            if (next->m_base[i] == key[0])
            {
                addValue(next->m_children[i], key.substr(1, key.size() - 1), value);
                return;
            }
    }
    addNode(next, key[0]);
    addValue(next->m_children.back(), key.substr(1, key.size() - 1), value);
}

template<typename ValueType>
void Trie<ValueType>::addNode(node<ValueType>* next, char base)
{
    next->m_children.push_back(addNode());
    next->m_base += base;
    next->m_nChildren++;
}

template<typename ValueType>
node<ValueType>* Trie<ValueType>::addNode()
{
    node<ValueType>* temp = new node<ValueType>;
    temp->m_nChildren = 0;
    return temp;
}

template<typename ValueType>
void Trie<ValueType>::doFind(node<ValueType>* next, std::vector<ValueType>& v, std::string key, bool exactMatchOnly) const
{
    if (key == "")
    {
        for (int i = 0; i < next->m_values.size(); i++)
            v.push_back(next->m_values[i]);
        return;
    }
    for (int i = 0; i < next->m_nChildren; i++)
    {
        if (next->m_base[i] == key[0])
            doFind(next->m_children[i], v, key.substr(1, key.size() - 1), exactMatchOnly);
        
        else if (exactMatchOnly == false && next != root)
            doFind(next->m_children[i], v, key.substr(1, key.size() - 1), true);
    }
}
