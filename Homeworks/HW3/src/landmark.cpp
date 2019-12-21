#include <string>
#include <iostream>
using namespace std;
////////////////////////////////////

class Landmark {
public:
    Landmark(string name);
    string name() const { return m_name; } //return name
    virtual string color() const { return "yellow"; } //returns color by type
    virtual string icon() const = 0; //returns icon by type
    virtual ~Landmark() {} //destructor is called, but shouldn't do anything
private:
    string m_name;
};

Landmark::Landmark(string name)
: m_name(name)
{}

//////////////////////////////////////////////////////
class Hotel : public Landmark {
public:
    Hotel(string name);
    virtual string icon() const { return "bed"; }
    ~Hotel();
};

Hotel::Hotel(string name)
: Landmark(name)
{}

Hotel::~Hotel()
{
    cout << "Destroying the hotel " << name() << "." << endl;
}

//////////////////////////////////////////////////////
class Hospital : public Landmark {
public:
    Hospital(string name);
    virtual string color() const { return "blue"; }
    virtual string icon() const { return "H"; }
    ~Hospital();
};

Hospital::Hospital(string name)
: Landmark(name)
{}

Hospital::~Hospital()
{
    cout << "Destroying the hospital " << name() << "." << endl;
}

//////////////////////////////////////////////////////
class Restaurant : public Landmark {
public:
    Restaurant(string name, int capacity);
    virtual string icon() const;
    ~Restaurant();
private:
    int m_capacity;
};

Restaurant::Restaurant(string name, int capacity)
: Landmark(name), m_capacity(capacity)
{}

string Restaurant::icon() const
{
    string size = (m_capacity < 40 ? "small" : "large");
    return (size + " knife/fork");
}

Restaurant::~Restaurant()
{
    cout << "Destroying the restaurant " << name() << "." << endl;
}

//////////////////////////////////////////////////////
