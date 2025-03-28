#ifdef PROFILER
#include "tracy_profiler/tracy/Tracy.hpp"
#endif

#include <iostream>

#include "systems/CollisionManager.hpp"

#include "systems/game_objects/EntityManager.hpp"
#include "entities/Entity.hpp"
#include "map/Map.hpp"
#include "map/Chunk.hpp"
#include "structures/Structure.hpp"

CollisionManager::~CollisionManager() {}

bool CollisionManager::checkRectanglesCollision(SDL_FRect rectA, SDL_FRect rectB)
{
    return !(rectA.x + rectA.w <= rectB.x ||
             rectA.x >= rectB.x + rectB.w ||
             rectA.y + rectA.h <= rectB.y ||
             rectA.y >= rectB.y + rectB.h);
}

bool CollisionManager::isPointInCollisionWithRectangle(float x, float y, SDL_FRect rect)
{
    return !(x <= rect.x ||
             x >= rect.x + rect.w ||
             y <= rect.y ||
             y >= rect.y + rect.h);
}

bool CollisionManager::isRectangleInCollisionWithSolidStructure(SDL_FRect rect)
{
    Chunk *chunk = this->map->getChunk(rect.x, rect.y);
    // structures
    if (chunk->isStructure(rect.x, rect.y))
    {
        Structure *structure = chunk->getStructure(rect.x, rect.y);
        if (checkRectanglesCollision(rect, structure->getHitBox()))
        {
            return structure->isSolid() ? true : false;
        }
    }
    return false;
}

SDL_FRect CollisionManager::handleCollisionsForEntity(Entity *entity, float newPosX, float newPosY)
{
    // entities
    std::vector<Entity *> entities = this->map->getEntityManager()->getPotentialEntities(entity);
    int size = entities.size();
    for (int i = 0; i < size; i++)
    {
        if ((entity != entities[i]) && (checkRectanglesCollision(entity->getHitBox(), entities[i]->getHitBox())))
        {
            entities[i]->onCollision(entity);
        }
    }
    SDL_FRect hitBox = entity->getHitBox();
    SDL_FRect newHitBox = {newPosX, newPosY, hitBox.w, hitBox.h};
    // structures
    // check destination for all 4 corners of the entity
    float newX, newY;
    Chunk *chunk;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            newX = newPosX + i * hitBox.w, newY = newPosY + j * hitBox.h;
            chunk = this->map->getChunk(newX, newY);
            // structures
            if (chunk->isStructure(newX, newY))
            {
                Structure *structure = chunk->getStructure(newX, newY);
                if (checkRectanglesCollision(newHitBox, structure->getHitBox()))
                {
                    structure->onCollision(entity);
                    return structure->isSolid() ? hitBox : newHitBox;
                }
            }
        }
    }
    return newHitBox;
}
