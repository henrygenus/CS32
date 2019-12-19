#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <typeinfo>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath) {}

int StudentWorld::init()
{
    
    Level lev(assetPath());
    setStatus(GWSTATUS_CONTINUE_GAME);
  
    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << getLevel() << ".txt";
    string levelFile = oss.str();
    
    
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Cannot find " << levelFile << " data file" << endl;
        return GWSTATUS_PLAYER_WON;
    }
    if (result == Level::load_fail_bad_format)
    {
        cerr << "Your level was improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        
        Level::MazeEntry ge;
        m_citizens = 0;
        for (int x = 0; x < LEVEL_WIDTH; x++)
        {
            for (int y = 0; y < LEVEL_HEIGHT; y++)
            {
                ge = lev.getContentsOf(x,y);
                setGrid(ge, x, y);
            }
        }
    }
    return getLevel() == LEVEL_WIN ? GWSTATUS_PLAYER_WON : GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setGrid(Level::MazeEntry ge, int x, int y)
{
    switch(ge)
    {
        case Level::wall:
            addActor(new Wall(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::exit:
            addActor(new Exit(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::pit:
            addActor(new Pit(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::landmine_goodie:
            addActor(new LandmineGoodie(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::vaccine_goodie:
            addActor(new Vaccine(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::gas_can_goodie:
            addActor(new GasCan(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::dumb_zombie:
            addActor(new DumbZombie(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::smart_zombie:
            addActor(new SmartZombie(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            break;
        case Level::citizen:
            addActor(new Citizen(x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
            m_citizens++;
            break;
        case Level::player:
            m_player = new Penelope(x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
            m_player->setWorld(*this);
            break;
        case Level::empty:
            break;
    }
}

int StudentWorld::move()
{
    display();
    m_player->doSomething();
    for (list<Actor*>::iterator p = m_board.begin(); p != m_board.end(); p++)
        (*p)->doSomething();
    for (list<Actor*>::iterator p = m_board.begin(); p != m_board.end(); p++)
        if (!(*p)->isAlive())
        {
            delete *p;
            p = m_board.erase(p);
        }
    if (!m_player->isAlive())
    {
        setStatus(GWSTATUS_PLAYER_DIED);
        decLives();
    }
    std::cout << "-------------" << std::endl;
    return (getLevel() == 99 ? GWSTATUS_PLAYER_WON : m_status);
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator p = m_board.begin(); p != m_board.end(); p++)
    {
        delete *p;
        m_board.erase(p);
    }
    if (m_player != nullptr)
        delete m_player;
}

void StudentWorld::addActor(Actor* actor)
{
    m_board.push_back(actor);
    m_board.back()->setWorld(*this);
}

void StudentWorld::display()
{
    ostringstream temp;
    temp.fill('0');
    int scoreSize = sqrt(pow(getScore(), 2));
    if (getScore() < 0)
        temp << "Score:  "  << "-" << setw(5) << scoreSize;
    else
        temp << "Score:  "  << setw(6) << scoreSize;
    temp << "  Level: "     << getLevel();
    temp << "  Lives:  "    << getLives();
    temp << "  Vaccines:  " << m_player->vaccineCount();
    temp << "  Flames:  "   << m_player->gasCount();
    temp << "  Mines:  "    << m_player->LMCount();
    temp << "  Infected:  " << m_player->infectionCount();
    string s = temp.str();
    setGameStatText(s);
}

bool StudentWorld::overlap(int x1, int y1, int x2, int y2)
{
    int dx = sqrt(pow(x1 - x2, 2));
    int dy = sqrt(pow(y1 - y2, 2));
    return (dx < SPRITE_SIZE && dy < SPRITE_SIZE);
}

void StudentWorld::iterate(Object* caller)
{
    for (std::list<Actor*>::iterator it = m_board.begin(); it != m_board.end(); it++)
    {
        if (*it == caller)
            continue;
        else if ((*it)->isAlive() && !(*it)->blocks() &&
                 caller->overlap((*it)->getX(), (*it)->getY(),
                                 caller->getX(), caller->getY()))
            caller->activate(*it);
    }
}

//will return isBlockedByWall as true if the object blocking is a wall
bool StudentWorld::isOpenSpace(int x, int y, Actor* caller)
{
    for (std::list<Actor*>::iterator it = m_board.begin(); it != m_board.end(); it++)
    {
        if (!(*it)->isAlive() || (*it) == caller)
            continue;
        else if (overlap((*it)->getX(), (*it)->getY(), x, y))
            if (caller != nullptr && ! (*it)->canOverlap())
                return false;
    }
    if (caller != nullptr && caller != m_player)
        if (overlap(m_player->getX(), m_player->getY(), x, y))
            return false;
    return true;
}

bool StudentWorld::isFlameBlockedAt(int x, int y)
{
    for (std::list<Actor*>::iterator it = m_board.begin(); it != m_board.end(); it++)
    {
        if (! (*it)->isAlive())
            continue;
        if (overlap(x, y, (*it)->getX(), (*it)->getY()) && (*it)->blocks())
            return true;
    }
    return false;
}

bool StudentWorld::isZombieVomitTriggeredAt(int x, int y)
{
    if (distance(x, y, m_player->getX(), m_player->getY()) <= OBJECT_RADIUS
        && m_player->infectionCount() == 0)
        return true;
    for (std::list<Actor*>::iterator it = m_board.begin(); it != m_board.end(); it++)
    {
        if (! (*it)->isAlive())
            continue;
        if (distance(x, y, (*it)->getX(), (*it)->getY()) <= OBJECT_RADIUS
            && ! (*it)->canOverlap()
            && ! (*it)->isThreat()
            && (*it)->infectionCount() == 0)
        return true;
    }
    return false;
}

Actor* StudentWorld::getClosestPersonTo(int x, int y, int threat)
{
    Actor* ptr = (threat ? nullptr : m_player);
    for (std::list<Actor*>::iterator it = m_board.begin(); it != m_board.end(); it++)
    {
       if (! (*it)->isAlive() || (*it)->canOverlap())
           continue;
       else if (ptr == nullptr || (!(*it)->blocks() && (*it)->isThreat() == threat
            && distance((*it)->getX(), (*it)->getY(), x, y)
                              < distance(ptr->getX(), ptr->getY(), x, y)))
           ptr = (*it);
    }
    return ptr;
}
