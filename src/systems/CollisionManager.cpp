#include "CollisionManager.hpp"

CollisionManager::CollisionManager() {}
CollisionManager::~CollisionManager() {}

void CollisionManager::init(std::vector<Entity *> entities, std::vector<StaticObject *> staticObjects)
{
    this->allEntities.insert(this->allEntities.end(), entities.begin(), entities.end());
    this->allStaticObjects.insert(this->allStaticObjects.end(), staticObjects.begin(), staticObjects.end());
}

bool CollisionManager::checkCollision(SDL_Rect rectA, SDL_Rect rectB)
{
    return !(rectA.x + rectA.w <= rectB.x ||
             rectA.x >= rectB.x + rectB.w ||
             rectA.y + rectA.h <= rectB.y ||
             rectA.y >= rectB.y + rectB.h);
}

SDL_Rect CollisionManager::handleCollisionsFor(Entity *entity, int newPosX, int newPosY)
{
    bool collision = false;
    SDL_Rect hitBox = entity->getHitBox();
    SDL_Rect newHitBox = {newPosX, newPosY, hitBox.w, hitBox.h};
    int size = this->allEntities.size();
    for (int i = 0; i < size && !collision; i++)
    {
        Entity *otherEntity = this->allEntities[i];
        if (entity != otherEntity && otherEntity->isSolid())
        {
            if (checkCollision(newHitBox, otherEntity->getHitBox()))
            {
                collision = true;
            }
        }
    }
    size = this->allStaticObjects.size();
    for (int i = 0; i < size && !collision; i++)
    {
        StaticObject *staticObject = this->allStaticObjects[i];
        if (staticObject->isSolid())
        {
            if (checkCollision(newHitBox, staticObject->getHitBox()))
            {
                printf("true\n");
                collision = true;
            }
        }
    }
    return collision ? newHitBox : hitBox;
}

void CollisionManager::addEntity(Entity *entity)
{
    this->allEntities.push_back(entity);
}