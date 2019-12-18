#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <sstream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    int m_length;
        //length of sequence
    string m_name;
        //name of genome
    string m_sequence;
        //the genome
};

//note: remove newlines here
GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
: m_length(int(sequence.length())), m_sequence(sequence), m_name(nm)
{}


bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    string name = "", sequence = "", line = "";
    for (;;)
    {
        sequence = "";
        if (genomeSource.get() == '>')
        {
            if ( ! getline(genomeSource, name) || ! genomeSource.peek())
                return false;
            while (genomeSource.peek() != EOF && genomeSource.peek() != '>')
            {
                line = "";
                getline(genomeSource, line);
                sequence += line;
            }
            Genome g = *new Genome(name, sequence);
            genomes.push_back(g);
            if (genomeSource.peek() == EOF)
                break;
        }
        else
            return false;
    }
    return true;
}

int GenomeImpl::length() const
{
    return int(m_sequence.length());
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    if (position + length > m_length)
        return false;
    string s;
    s.assign(m_sequence.substr(position, length));
    if (s.size() != length)
        return false;
    else
    {
        fragment = "";
        fragment.assign(s);
        return true;
    }
}


// ******************** Genome functions ***********************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}

