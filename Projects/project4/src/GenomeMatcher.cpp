#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    ~GenomeMatcherImpl();
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    int m_minSearchLength;
        //depth of trie/length of shortest thing that can be searched for
    Trie<DNAMatch*>* m_sequences;
        //tree pointer which holds genome index and position in genomes
    vector<const Genome*> m_genomes;
        //vector of all genomes passed into the trie
    void determineLength(DNAMatch &d, const string& fragment, bool exactMatchOnly) const;
        //iterates through the fragment, comparing it to the relevant sequence
        //sets length equal to the number of characters that match
    static int compareGenome(const void* a, const void* b);
        //compares length with name as tiebreaker: for qsort
    static bool compareDNA(const DNAMatch* G1, const DNAMatch* G2);
    //compares length of match with name as tiebreaker but w/ true:false
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
: m_minSearchLength(minSearchLength), m_sequences(new Trie<DNAMatch*>)
{}

GenomeMatcherImpl::~GenomeMatcherImpl()
{
    delete m_sequences;
    for (int i = 0; i < m_genomes.size(); i++)
        delete m_genomes[i];
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    const Genome* ptr = new Genome(genome);
    m_genomes.push_back(ptr);
    int i = 0; string fragment; DNAMatch* p;
    
    while (genome.extract(i, minimumSearchLength(), fragment)
           && fragment.length() == minimumSearchLength())
    {
        p = new DNAMatch;
        p->genomeName = genome.name();
        p->position = i;
        m_sequences->insert(fragment, p);
        delete p;
        i++;
        fragment = "";
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minSearchLength;
}


//try using direct rather than dynamic
bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    if (fragment.length() < minimumSearchLength())
        return false;
    if (minimumLength < minimumSearchLength())
        return false;
    
    list<DNAMatch*> output;
    vector<DNAMatch*> temp = m_sequences->find(fragment.substr(0, minimumSearchLength()), exactMatchOnly);
    for (int i = 0; i < temp.size(); i++)
    {
        determineLength(*temp[i], fragment, exactMatchOnly);
        output.push_back(temp[i]);
    }
   
    if (output.empty())
        return false;
    
    output.sort(compareDNA);
    matches.clear();
    while (output.size() != 0)
    {
        DNAMatch* ptr = output.front();
        output.pop_front();
        if (matches.size() == 0 || matches.back().genomeName != ptr->genomeName)
            matches.push_back(*ptr);
        else if (matches.back().length < ptr->length)
        {
            matches.pop_back();
            matches.push_back(*ptr);
        }
    }
    return true;
}

void GenomeMatcherImpl::determineLength(DNAMatch &d, const string& fragment, bool exactMatchOnly) const
{
    bool SNiPCatch = (!exactMatchOnly);
    int length = 0;
    std::string sequence = "";
    for (int i = 0; i < m_genomes.size(); i++)
        if (d.genomeName == m_genomes[i]->name())
        {
            if (!m_genomes[i]->extract(d.position, (int)fragment.length(), sequence))
                m_genomes[i]->extract(d.position, m_genomes[i]->length() - d.position, sequence);
            break;
        }
    
    for (int j = 0; j < sequence.length(); j++)
    {
        if (fragment[j] == sequence[j])
            length++;
        else if (SNiPCatch)
        {
            SNiPCatch = false;
            length++;
        }
        else
            break;
    }
}

bool GenomeMatcherImpl::compareDNA(const DNAMatch* G1, const DNAMatch* G2)
{
    return (G1->length < G2->length ? false
            : G1->length > G2->length ? true
            : G1->genomeName < G2->genomeName ? false
            : G1->genomeName > G1->genomeName ? true
            : false);
}


bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    int numLoops = query.length()/fragmentMatchLength;
    if (numLoops == 0 || fragmentMatchLength < minimumSearchLength())
        return false;
    GenomeMatch* matches = new GenomeMatch[(int) m_genomes.size()];
    
    for (size_t i = 0; i < m_genomes.size(); i++)
    {
        matches[i].genomeName = m_genomes[i]->name();
        matches[i].percentMatch = 0;
    }
    
    vector<DNAMatch> v;
    string fragment;
    
    for (int i = 0; i < numLoops; i++)
    {
        query.extract(i*fragmentMatchLength, fragmentMatchLength, fragment);
        findGenomesWithThisDNA(fragment, fragmentMatchLength, exactMatchOnly, v);
    }
    
    for (int j = 0; j < (int) v.size(); j++)
    {
        for (int k = 0; k < (int) m_genomes.size(); k++)
            if (v[k].genomeName.compare(matches[j].genomeName) == 0)
                matches[j].percentMatch += 1;
    }

    for (int i = 0; i < m_genomes.size(); i++)
    {
        matches[i].percentMatch *= 100.0/numLoops;
    }
    
    qsort(matches, m_genomes.size(), sizeof(GenomeMatch), compareGenome);
    size_t end = m_genomes.size();
    
    for (int i = 0; i < m_genomes.size(); i++)
        if (matches[i].percentMatch < matchPercentThreshold)
        {
            end = i;
        }
    
    for (int i = 0; i < end; i++)
        results.push_back(matches[i]);
    
    delete[] matches;
            
    return (end != 0);
}



int GenomeMatcherImpl::compareGenome(const void* a, const void* b)
{
    int p1 = ((struct GenomeMatch*) a)->percentMatch;
    int p2 = ((struct GenomeMatch*) b)->percentMatch;
    string n1 = ((struct GenomeMatch*) a)->genomeName;
    string n2 = ((struct GenomeMatch*) b)->genomeName;
    return (p1 < p2 ? -1
          : p1 > p2 ? 1
          : n1 < n2 ? -1
          : n1 < n2 ? 1
          : 0);
}

// ******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
