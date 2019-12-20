#include "Actor.h"
#include "StudentWorld.h"
#include <list>
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

const int MOVEMENT_SIZE = 4;
const int ZOMBIE_MOVE_LENGTH = 1;
const int CITIZEN_MOVE_LENGTH = 2;
const int LANDMINES = -2;
const int GAS = -3;
const int VACCINES = -4;

#define IN_LINE -1


double distance(int x1, int y1, int x2, int y2)
{
    double dx2 = pow(x1 - x2, 2);
    double dy2 = pow(y1 - y2, 2);
    return (sqrt(dx2 + dy2));
}

// //////////////////////////////
// ////////   ACTOR    //////////
// //////////////////////////////

void Actor::increaseScore(int score)
{
    getWorld()->increaseScore(score);
}

void Actor::playSound(int ID)
{
    getWorld()->playSound(ID);
}

// //////////////////////////////
// ////////   PERSON   //////////
// //////////////////////////////

//-1 marks equal
int Person::FindDirectionsToward(Actor* closestPerson)
{
    int dx = closestPerson->getX() - getX();
    int dy = closestPerson->getY() - getY();
    dx = ( dx > 0 ? right
             : dx < 0 ? left
             : IN_LINE);
    dy = ( dy > 0 ? up
             : dy < 0 ? down
             : IN_LINE);

    if (dx == IN_LINE && dy != IN_LINE)
         return dy;
     else if (dy == IN_LINE && dx != IN_LINE)
         return dx;
     else
         return (randInt(0, 1) ? dx : dy);
}

bool Person::tryToMove(int dest_x, int dest_y)
{
    if (getWorld()->isOpenSpace(dest_x, dest_y, this))
    {
        moveTo(dest_x, dest_y);
        return true;
    }
    return false;
}



// //////////////////////////////
// ////////    HUMAN   //////////
// //////////////////////////////

void Human::doSomething()
{
    
    if (infected)
        m_infectionCount++;
    std::cout << m_infectionCount << std::endl;
    if (m_infectionCount == 500)
        fatalInfection();
}

// //////////////////////////////
// ////////  PENELOPE  //////////
// //////////////////////////////

void Penelope::increment(int mode)
{
    switch(mode)
    {
        case VACCINES:
            m_vaccines++;
            break;
        case GAS:
            m_gas += 5;
            break;
        case LANDMINES:
            m_landmines += 2;
            break;
    }
}

void Penelope::decrement(int mode)
{
    switch(mode)
    {
        case VACCINES:
            m_vaccines--;
            break;
        case GAS:
            m_gas--;
            break;
        case LANDMINES:
            m_landmines--;
            break;
    }
}

void Penelope::doSomething()
{
    if ( ! isAlive())
        return;
    Human::doSomething();
    int key; bool movement = false;
    int x = getX(), y = getY();
    if (getWorld()->getKey(key))
    {
        switch (key)
        {
            case KEY_PRESS_SPACE:
                if (gasCount() != 0)
                {
                    throwFlame();
                    decrement(GAS);
                }
                break;
            case KEY_PRESS_TAB:
                if (LMCount() != 0)
                {
                    getWorld()->addActor(new Landmine(getX(), getY()));
                    decrement(LANDMINES);
                }
                break;
            case KEY_PRESS_ENTER:
                if (vaccineCount() != 0)
                {
                    cure();
                    decrement(VACCINES);
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                y += MOVEMENT_SIZE;
                movement = true;
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                y -= MOVEMENT_SIZE;
                movement = true;
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                x -= MOVEMENT_SIZE;
                movement = true;
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                x += MOVEMENT_SIZE;
                movement = true;
                break;
            default:
                break;
        }
        
        //check for overlapping "living" characters
        if (movement)
          tryToMove(x, y);
    }
}

void Penelope::throwFlame()
{
    playSound(SOUND_PLAYER_FIRE);
    int dir = getDirection(), x = getX(), y = getY();
    for (int i = 0; i < 3; i++)
    {
        int dest_x = x + (i+1) * cos(dir * PI/180) * SPRITE_WIDTH;
        int dest_y = y + (i+1) * sin(dir * PI/180) * SPRITE_HEIGHT;
        if (! getWorld()->isFlameBlockedAt(dest_x, dest_y))
            getWorld()->addActor(new Flame(x + i * cos(dir * PI/180) * SPRITE_WIDTH,
                                           y + i * sin(dir * PI/180) * SPRITE_HEIGHT, dir));
        else
            break;
    }
}

// //////////////////////////////
// ////////  CITIZENS  //////////
// //////////////////////////////

void Citizen::burn()
{
    setDead();
    playSound(SOUND_CITIZEN_DIE);
    getWorld()->citizenGone();
}

void Citizen::infect()
{
    if(infectionCount() == 0)
    {
        Human::infect();
        playSound(SOUND_CITIZEN_INFECTED);
    }
}

void Citizen::exit()
{
    setDead();
    playSound(SOUND_CITIZEN_SAVED);
    getWorld()->citizenGone();
    increaseScore(1500);
}

void Citizen::setDead()
{
    Actor::setDead();
    increaseScore(-1000);
}

bool Citizen::distanceIncreases(int x, int y, int dist_z) const
{
    Actor* closest = getWorld()->getClosestZombieTo(x, y);
    if (closest == nullptr)
        return true;
    int dist_zNew = distance(closest->getX(), closest->getY(), x, y);
    if (dist_zNew > 80 || dist_zNew > dist_z)
        return true;
    return false;
}

void Citizen::doSomething()
{
    Human::doSomething();
    changeMove();
    if ( ! isAlive() || ! canMove())
        return;
    Actor* closestZombie = getWorld()->getClosestZombieTo(getX(), getY());
    int dist_p = distance(getX(), getY(),
                      getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY());
    int dist_z = distance(closestZombie->getX(), closestZombie->getY(), getX(), getY());
    if (dist_p < dist_z) // Penelope is closer than any zombie
    {
        setDirection(FindDirectionsToward(getWorld()->getPlayer()));
        int dest_x = getX() + CITIZEN_MOVE_LENGTH * cos(getDirection() * PI/180);
        int dest_y = getY() + CITIZEN_MOVE_LENGTH * sin(getDirection() * PI/180);
        if (tryToMove(dest_x, dest_y))
            return;
    }
    setDirection(180 + FindDirectionsToward(closestZombie));
    int max = INT_MIN, x = getX(), y = getY(), dist;
    for (int i = 0; i < 360; i += 90)
    {
        setDirection(i);
        int dest_x = x + CITIZEN_MOVE_LENGTH * cos(getDirection() * PI/180);
        int dest_y = y + CITIZEN_MOVE_LENGTH * sin(getDirection() * PI/180);
        dist = distance(closestZombie->getX(), closestZombie->getY(), dest_x, dest_y);
        if (dist > max && distanceIncreases(dest_x, dest_y, dist_z))
            if (tryToMove(dest_x, dest_y))
            {
                max = dist;
                setDirection(i);
            }
    }
}

void Citizen::fatalInfection()
{
    setDead();
    playSound(SOUND_ZOMBIE_BORN);
    if (randInt(1, 10) > 7)
        getWorld()->addActor(new SmartZombie(getX(), getY()));
    else
        getWorld()->addActor(new DumbZombie(getX(), getY()));
    getWorld()->citizenGone();
}

// //////////////////////////////
// ////////   ZOMBIES  //////////
// //////////////////////////////

void Zombie::setDead()
{
    Actor::setDead();
    playSound(SOUND_ZOMBIE_DIE);
    increaseScore(1000);
    if (hasVaccine)
        attemptToThrowVaccine();
}

void DumbZombie::attemptToThrowVaccine()
{
    int dir = 90 * randInt(0, 3);
    int dest_x = getX() + cos(dir * PI/180) * SPRITE_WIDTH;
    int dest_y = getY() + sin(dir * PI/180) * SPRITE_HEIGHT;
    if (getWorld()->isOpenSpace(dest_x, dest_y, nullptr))
        getWorld()->addActor(new Vaccine(dest_x, dest_y));
}

void Zombie::doSomething()
{
    changeMove();
    if ( ! isAlive() || canMove() )
        return;
    if (!randInt(0, 3) && tryToVomit())
        return;
    if (m_movementPlanDistance == 0)
        setMovementPlan();
    int dest_x = getX() + ZOMBIE_MOVE_LENGTH * cos(getDirection() * PI/180);
    int dest_y = getY() + ZOMBIE_MOVE_LENGTH * sin(getDirection() * PI/180);
    if (tryToMove(dest_x, dest_y))
        m_movementPlanDistance--;
    else
        setMovementPlanDistance(0);
}

bool Zombie::tryToVomit()
{
    int dest_x = getX() + cos(getDirection() * PI/180) * SPRITE_WIDTH;
    int dest_y = getY() + sin(getDirection() * PI/180) * SPRITE_HEIGHT;
    if (getWorld()->isZombieVomitTriggeredAt(dest_x, dest_y))
    {
        getWorld()->addActor(new Vomit(getX(), getY(), getDirection()));
        playSound(SOUND_ZOMBIE_VOMIT);
        return true;
    }
    return false;
}

void DumbZombie::setMovementPlan()
{
    setMovementPlanDistance(randInt(3, 10));
    setDirection(randInt(0, 3) * 90);
}

void SmartZombie::setMovementPlan()
{
    setMovementPlanDistance(randInt(3, 10));
    Actor* closestPerson = getWorld()->getClosestCitizenTo(getX(), getY());
    if (distance(closestPerson->getX(), closestPerson->getY(), getX(), getY()) < 80)
        setDirection(FindDirectionsToward(closestPerson));
    else
        setDirection(randInt(0, 3) * 90);
}

// //////////////////////////////
// ////////   OBJECTS  //////////
// //////////////////////////////

bool Object::overlap(int x1, int y1, int x2, int y2) const
{
    return (distance(x1, y1, x2, y2) <= 10);
}

void Object::doSomething()
{
    if (!isAlive())
        return;

    int x = getWorld()->getPlayer()->getX();
    int y = getWorld()->getPlayer()->getY();
    if (overlap(x, y, getX(), getY()))
        activate(getWorld()->getPlayer());

    getWorld()->iterate(this); 
}

void Exit::activate(Actor* actor)
 {
    if (getWorld()->numCitizens() > 0)
    {
        actor->exit();
        increaseScore(500);
    }
    else if (actor == getWorld()->getPlayer())
    {
        getWorld()->setStatus(GWSTATUS_FINISHED_LEVEL);
        getWorld()->playSound(SOUND_LEVEL_FINISHED);
    }
}

void Pit::activate(Actor* actor)
{
    actor->setDead();
}

void Landmine::doSomething()
{
    if (! isAlive())
        return;
    else if (! isActive())
    {
        if (--m_safetyTicks == 0)
            makeActive();
        return;
    }
    Object::doSomething();
}

void Landmine::activate(Actor* actor)
{
    if (!isAlive())
        return;
    setDead();
    playSound(SOUND_LANDMINE_EXPLODE);
    //set fire to spaces around it with orientation UP
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (getWorld()->isFlameBlockedAt(getX() + i * SPRITE_WIDTH,
                                             getY() + j * SPRITE_HEIGHT))
            getWorld()->addActor(new Flame(getX() + i * SPRITE_WIDTH,
                                           getY() + (j-1) * SPRITE_HEIGHT, up));
        }
    }
    getWorld()->addActor(new Pit(getX() * SPRITE_WIDTH, getY() * SPRITE_HEIGHT));
}

// //////////////////////////////
// //////// PROJECTILES//////////
// //////////////////////////////

void Projectile::doSomething()
{
    if (time() == 0)
        setDead();
    else
        decrementTime();
    Object::doSomething();
}

void Flame::activate(Actor* actor)
{
    actor->burn();
}

void Vomit::activate(Actor* actor)
{
    actor->infect();
}

// //////////////////////////////
// ////////   GOODIES  //////////
// //////////////////////////////

void Goodie::doSomething()
{
    if (!isAlive())
        return;
    int x1 = getWorld()->getPlayer()->getX();
    int y1 = getWorld()->getPlayer()->getY();
    int x2 = getX();
    int y2 = getY();
    
    if (overlap(x1, y1, x2, y2))
    {
        increaseScore(50);
        activate(nullptr);
        playSound(SOUND_GOT_GOODIE);
    }
}


void Vaccine::activate(Actor* actor)
{
    setDead();
    getWorld()->getPlayer()->increment(VACCINES);
}

void GasCan::activate(Actor* actor)
{
    setDead();
    getWorld()->getPlayer()->increment(GAS);
}

void LandmineGoodie::activate(Actor* actor)
{
    setDead();
    getWorld()->getPlayer()->increment(LANDMINES);
}
