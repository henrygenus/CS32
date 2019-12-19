#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GraphObject.h"
#include "Level.h"
#include <string>
#include <list>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

#define SPRITE_SIZE 16
#define OBJECT_RADIUS 10

class Penelope;
class Actor;
class Object;

class StudentWorld : public GameWorld
{
public:
    //presets
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld() { cleanUp(); }
    
    //if an object is activated, iterate over board and actiavate
    void iterate(Object* caller);
    
    //getters
    Penelope* getPlayer() { return m_player; }
    int numCitizens() { return m_citizens; }
    Actor* getClosestCitizenTo(int x, int y) {return getClosestPersonTo(x, y, false);}
    Actor* getClosestZombieTo(int x, int y) {return getClosestPersonTo(x, y, true);}
    Actor* getClosestPersonTo(int x, int y, int threat);
    bool isFlameBlockedAt(int x, int y);
    bool isZombieVomitTriggeredAt(int x, int y);
    
    //booleans
    //get whether a space is empty (can be moved to) & if it is blocked by a wall
    bool isOpenSpace(int x, int y, Actor* actor);
    
    //setters
    void setStatus(int status) { m_status = status; }
    void citizenGone() { m_citizens--; }
    void addActor(Actor* actor);
    
private:
    //data members
    Penelope* m_player;
    std::list<Actor*> m_board; //maybe vector
    int m_citizens;
    int m_status;
    
    //helper-setters
    void setGrid(Level::MazeEntry ge, int x, int y);
    bool overlap(int x1, int y1, int x2, int y2);
    void display();
};

#endif // STUDENTWORLD_H_


