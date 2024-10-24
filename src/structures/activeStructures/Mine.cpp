#include "structures/activeStructures/Mine.hpp"

Mine::Mine(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP, Faction *faction) : ActiveStructure(texture, collisionManager, entityManager, hitBox, HP, true, faction) {}
Mine::~Mine() {}

void Mine::update()
{
    if (this->active)
    {
    }
}