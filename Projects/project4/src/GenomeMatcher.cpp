#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

struct Match
{
    int pos;
    int genomePos;
};

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
    Trie<Match*>* m_sequences;
        //tree pointer which holds genome index and position in genomes
    vector<const Genome*> m_genomes;
        //vector of all genomes passed into the trie
    vector<Match*> m_matches;
        //vector of all matches: (struct: holds genome index and position)
    void determineLength(Match &d, DNAMatch* p, const string& fragment, bool exactMatchOnly) const;
        //iterates through the fragment, comparing it to the relevant sequence
        //sets length equal to the number of characters that match
    void add(list<DNAMatch*> &l, DNAMatch* p) const;
        //adds DNAMatch to list if it is longer than the others w/ same name
        //deletes all shorter matches from the same genome
    static int compareGenome(const void* a, const void* b);
        //compares length with name as tiebreaker: for qsort
    static bool compareDNA(const DNAMatch* G1, const DNAMatch* G2);
    //compares length of match with name as tiebreaker but w/ true:false
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
: m_minSearchLength(minSearchLength), m_sequences(new Trie<Match*>)
{}

GenomeMatcherImpl::~GenomeMatcherImpl()
{
    delete m_sequences;
    if (m_matches.empty())
        return;
    for (int i = 0; i < m_matches.size(); i++)
        delete m_matches[i];
    for (int i = 0; i < m_genomes.size(); i++)
        delete m_genomes[i];
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    const Genome* ptr = new Genome(genome);
    m_genomes.push_back(ptr);
    int i = 0; string fragment; Match* p;
    
    while (genome.extract(i, minimumSearchLength(), fragment)
           && fragment.length() >= minimumSearchLength())
    {
        p = new Match;
        p->genomePos = (int) m_genomes.size() - 1;
        p->pos = i;
        m_sequences->insert(fragment, p);
        m_matches.push_back(p);
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
    
    list<DNAMatch*> l;
    vector<Match*> temp = m_sequences->find(fragment.substr(0, minimumSearchLength()), exactMatchOnly);
    for (int i = 0; i < temp.size(); i++)
    {
        DNAMatch* p = new DNAMatch;
        p->genomeName = m_genomes[temp[i]->genomePos]->name();
        p->position = temp[i]->pos;
        
        determineLength(*temp[i], p, fragment, exactMatchOnly);
        if (p->length < minimumLength)
        {
            delete p;
            continue;
        }
        else
            add(l, p);
    }
   
    if (l.empty())
        return false;
    
    l.sort(compareDNA);
    matches.clear();
    for (auto it = l.begin(); it != l.end(); it++)
    {
        matches.push_back(**it);
        delete *it;
    }
    
        return true;
}

void GenomeMatcherImpl::determineLength(Match &d, DNAMatch* p, const string& fragment, bool exactMatchOnly) const
{
    bool SNiPCatch = (!exactMatchOnly);
    p->length = 0;
    std::string sequence = "";
    m_genomes[d.genomePos]->extract(p->position, (int)fragment.length(), sequence);

    for (int j = 0; j < fragment.length(); j++)
    {
        if (fragment[j] == sequence[j])
            p->length++;
        else if (SNiPCatch)
        {
            SNiPCatch = false;
            p->length++;
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

void GenomeMatcherImpl::add(list<DNAMatch*> &l, DNAMatch* p) const
{
    bool toAdd = true;
    for (auto it = l.begin(); it != l.end(); it++)
    {
        if ((*it)->genomeName == p->genomeName)
        {
            if (p->length > (*it)->length)
            {
                delete *it;
                it = l.erase(it);
            }
            else
            {
                toAdd = false;
                break;
            }
        }
    }
    if (toAdd)
        l.push_back(p);
    else
        delete p;
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
