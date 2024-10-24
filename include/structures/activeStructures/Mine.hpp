#ifndef mine_hpp
#define mine_hpp

#include "ActiveStructure.hpp"

class Faction;

class Mine : public ActiveStructure
{
public:
    Mine(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP, Faction *faction);
    ~Mine();

    void update();

private:
};

#endif