#include "systems/CollisionManager.hpp"

#include <iostream>
#include "systems/EntityManager.hpp"
#include "entities/Entity.hpp"
#include "map/Map.hpp"
#include "map/Chunk.hpp"
#include "structures/Structure.hpp"

CollisionManager::CollisionManager() {}
CollisionManager::~CollisionManager() {}

void CollisionManager::init(Map *map, EntityManager *entityManager)
{
    this->map = map;
    this->entityManager = entityManager;
}

bool CollisionManager::checkCollision(SDL_Rect rectA, SDL_Rect rectB)
{
    return !(rectA.x + rectA.w <= rectB.x ||
             rectA.x >= rectB.x + rectB.w ||
             rectA.y + rectA.h <= rectB.y ||
             rectA.y >= rectB.y + rectB.h);
}
bool CollisionManager::checkCollisionFromCoordinates(int x, int y, SDL_Rect rect)
{
    return !(x <= rect.x ||
             x >= rect.x + rect.w ||
             y <= rect.y ||
             y >= rect.y + rect.h);
}
bool CollisionManager::checkCollisionWithSolidStructure(SDL_Rect rect)
{
    if (this->map->isChunkGenerated(rect.x, rect.y))
    {
        Chunk *chunk = this->map->getChunk(rect.x, rect.y);
        // structures
        if (chunk->isStructure(rect.x, rect.y))
        {
            Structure *structure = chunk->getStructure(rect.x, rect.y);
            if (checkCollision(rect, structure->getHitBox()))
            {
                return structure->isSolid() ? true : false;
            }
        }
    }
    return false;
}
SDL_Rect CollisionManager::handleCollisionsFor(Entity *entity, int newPosX, int newPosY)
{
    // entities
    std::vector<Entity *> entities = this->entityManager->getPotentialEntities(entity);
    int size = entities.size();
    for (int i = 0; i < size; i++)
    {
        if ((entity != entities[i]) && (checkCollision(entity->getHitBox(), entities[i]->getHitBox())))
        {
            entities[i]->onCollision(entity);
        }
    }
    bool collision = false;
    SDL_Rect hitBox = entity->getHitBox();
    SDL_Rect newHitBox = {newPosX, newPosY, hitBox.w, hitBox.h};
    // structures
    if (this->map->isChunkGenerated(newPosX, newPosY))
    {
        // check destination for all 4 corners of the entity
        int newX, newY;
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
                    if (checkCollision(newHitBox, structure->getHitBox()))
                    {
                        structure->onCollision(entity);
                        return structure->isSolid() ? hitBox : newHitBox;
                    }
                }
            }
        }
    }
    return newHitBox;
}