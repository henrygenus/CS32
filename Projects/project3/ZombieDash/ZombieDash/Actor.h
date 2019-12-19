#ifndef ACTOR_H_
#define ACTOR_H_

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#include "GraphObject.h"
#include "GameWorld.h"
#include <cmath>

const double PI = 4 * atan(1);

class StudentWorld;
double distance(int x1, int y1, int x2, int y2);

class Actor : public GraphObject
{
public:
    //utilities
    Actor(int IID, int x, int y, int depth = 0, int dir = right)
        : GraphObject(IID, x, y, dir, depth), m_living(true) {}
    void increaseScore(int score);
    void playSound(int ID);
    virtual ~Actor() {};
    
    virtual void doSomething() = 0;
    
    //setters
    void setWorld(StudentWorld& world) { m_world = &world; }
    void setDead() { m_living = false; }
   
    
    //getters
    StudentWorld* getWorld() const { return m_world; }
    bool isAlive() { return m_living; }
    virtual bool blocksFlame() const = 0;
    virtual bool canOverlap() const = 0;
    virtual bool isThreat() const { return false; }
    virtual int infectionCount() const { return -1; }

    
    //general virtuals
    virtual void infect() {};
    virtual void burn() {};
    virtual void exit() {};
   

private:
    StudentWorld* m_world;
    bool m_living;
};
// //////////////////////////////
// ////////    WALLS   //////////
// //////////////////////////////

class Wall : public Actor
{
public:
    //utilities
    Wall(int x, int y)
    : Actor(IID_WALL, x, y) {}
    
    virtual void doSomething() {};
   
    //getters
    virtual bool blocksFlame() const { return true; }
    virtual bool canOverlap() const { return false; }
};

// //////////////////////////////
// ////////   PEOPLE   //////////
// //////////////////////////////

class Person : public Actor
{
public:
    //utilities
    Person(int ID, int x, int y)
        : Actor(ID, x, y, 0), m_move(false) {}
    virtual ~Person() {}
    
    //setters
    virtual void burn() { setDead(); }
    void changeMove() { m_move = (m_move == true ? false : true); }
    
    //getters
    virtual bool canMove() const { return m_move; }
    virtual bool blocksFlame() const { return false; }
    virtual bool canOverlap() const { return false; }

    //general virtuals
    void FindDirectionsToward
        (Actor* closestPerson, int &dx, int &dy);
    virtual bool tryToMove(int dest_x, int dest_y);
   
private:
    bool m_move;
};

// //////////////////////////////
// ////////   HUMANS   //////////
// //////////////////////////////

class Human : public Person
{
public:
    //utilities
    Human(int ID, int x, int y)
        : Person(ID, x, y),
        infected(false), m_infectionCount(0) {}
     virtual ~Human() {}
    
    virtual void doSomething();
    
    //setters
    virtual void infect() { infected = true; }
    virtual void cure() { infected = false; m_infectionCount = 0; }

    //getters
    virtual int infectionCount() const { return m_infectionCount; }
    
    //general virtuals
    virtual void fatalInfection() = 0;
private:
    bool infected;
    int m_infectionCount;
};

class Penelope : public Human
{
public:
    //utilities
    Penelope(int x, int y)
        : Human(IID_PLAYER, x, y),
        m_landmines(0), m_gas(0) , m_vaccines(0) {}
    
    virtual void doSomething();
   
    //setters
    void increment(int mode);
    void decrement(int mode);
    virtual void setDead() { playSound(SOUND_PLAYER_DIE); Actor::setDead(); }
   
    //getters
    int LMCount() const { return m_landmines; }
    int gasCount() const { return m_gas; }
    int vaccineCount() const { return m_vaccines; }
   
    //general virtuals
    void throwFlame();
    virtual void fatalInfection() { setDead(); }

private:
    int m_landmines;
    int m_gas;
    int m_vaccines;
};

//can use same column checker as smartzombie but with bool
class Citizen : public Human
{
public:
    //utilities
    Citizen(int x, int y)
        : Human(IID_CITIZEN, x, y) {}
    
    virtual void doSomething();

    //setters
    virtual void setDead();
    virtual void burn();
    virtual void infect();
    
    //general virtuals
    virtual void fatalInfection();
    virtual void exit();
    
    //additional checks
    bool distanceIncreases(int x, int y, int dist_z) const;
};

// //////////////////////////////
// ////////  ZOMBIES   //////////
// //////////////////////////////

class Zombie : public Person
{
public:
    //utilities
    Zombie(int x, int y)
        : Person(IID_ZOMBIE, x, y), m_movementPlanDistance(0) {}
    virtual ~Zombie() {}
    
    virtual void doSomething();
    
    //setters
    virtual void setDead();
    virtual void burn() { setDead(); }
    virtual void setMovementPlanDistance(int d) { m_movementPlanDistance = d; }
    
    //getters
    virtual bool isThreat() const { return true; }
    
    //general virtuals
    virtual void setMovementPlan() = 0;
    virtual void attemptToThrowVaccine() = 0;
    virtual bool tryToVomit();
   
private:
    int m_movementPlanDistance;
};


class DumbZombie : public Zombie
{
public:
    //utilities
    DumbZombie(int x, int y)
        : Zombie(x, y) {}
    
    virtual void setMovementPlan();
    virtual void attemptToThrowVaccine();
};

class SmartZombie : public Zombie
{
public:
    //utilities
    SmartZombie(int x, int y)
        : Zombie(x, y) {}
    
    virtual void setMovementPlan();
    
    //setters
    virtual void setDead() { Zombie::setDead(); increaseScore(1000); }
    
    //general virtuals
    virtual void attemptToThrowVaccine() {}
};

// //////////////////////////////
// ////////   OBJECTS  //////////
// //////////////////////////////

class Object : public Actor
{
public:
    //utilities
    Object(int ID, int x, int y, int depth, bool isActive = true)
        : Actor(ID, x, y, depth), m_isActive(isActive) {}
    virtual ~Object() {}
    
    virtual void doSomething();
  
    //setters
    virtual void makeActive() { m_isActive = true; }
    virtual void activate(Actor* actor) = 0;

    //getters
    virtual bool canOverlap() const { return true; }
    virtual bool isActive() const { return m_isActive; }
    virtual bool blocksFlame() const { return false; }
    bool overlap(int x1, int y1, int x2, int y2) const;
    
private:
    bool m_isActive;
};


class Exit : public Object
{
public:
    Exit(int x, int y)
    : Object(IID_EXIT, x, y, 1) {}
    virtual void activate(Actor* actor);
    virtual void setDead() {};
};

class Pit : public Object
{
public:
    Pit(int x, int y)
    : Object(IID_PIT, x, y, 0) {}
    virtual void activate(Actor* actor);
    virtual void setDead() {};
};

class Landmine : public Object
{
public:
    Landmine(int x, int y)
        : Object(IID_LANDMINE, x, y, 1, false), m_safetyTicks(30) {}
    virtual void activate(Actor* actor);
    virtual void doSomething();
    virtual void burn() { activate(nullptr); }
private:
    int m_safetyTicks;
};

// //////////////////////////////
// /////// PROJECTILES //////////
// //////////////////////////////

class Projectile : public Object
{
public:
    //utilities
    Projectile(int ID, int x, int y, int dir)
        : Object(ID, x + SPRITE_WIDTH * cos(dir * PI/180),
                     y + SPRITE_HEIGHT * sin(dir * PI/180), 0), m_time(2) {}
    virtual ~Projectile() {}
    
    virtual void doSomething();
    
    //setters
    void decrementTime() { m_time--; }
    
    //getters
    int time() { return m_time; }
private:
    int m_time;
};

class Flame : public Projectile
{
public:
    Flame(int x, int y, int dir)
    : Projectile(IID_FLAME, x, y, dir) {}
    virtual void activate(Actor* actor);
};

class Vomit : public Projectile
{
public:
    Vomit(int x, int y, int dir)
    : Projectile(IID_VOMIT, x, y, dir) {}
    virtual void activate(Actor* actor);
};


// //////////////////////////////
// ////////   GOODIES  //////////
// //////////////////////////////

class Goodie : public Object
{
public:
    Goodie(int ID, int x, int y)
    : Object(ID, x, y, 1) {}
    virtual ~Goodie() {}
    
    virtual void doSomething();
};

class Vaccine : public Goodie
{
    public:
    Vaccine(int x, int y)
        : Goodie(IID_VACCINE_GOODIE, x, y) {}
    virtual void activate(Actor* actor);
};

class GasCan : public Goodie
{
public:
    GasCan(int x, int y)
        : Goodie(IID_GAS_CAN_GOODIE, x, y) {}
    virtual void activate(Actor* actor);
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(int x, int y)
        : Goodie(IID_LANDMINE_GOODIE, x, y) {}
    virtual void activate(Actor* actor);
};


#endif // ACTOR_H_
