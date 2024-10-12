#include "ActiveStructure.hpp"

#include "../../actions/Action.hpp"

ActiveStructure::ActiveStructure(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP, bool solid) : Structure(texture, hitBox, HP, solid)
{
    this->collisionManager = collisionManager;
    this->entityManager = entityManager;
    this->active = true;
}
ActiveStructure::~ActiveStructure()
{
    int size = this->actions.size();
    for (int i = 0; i < size; i++)
    {
        delete this->actions[i];
    }
}

void ActiveStructure::onLeftClick()
{
    this->active = !this->active;
}