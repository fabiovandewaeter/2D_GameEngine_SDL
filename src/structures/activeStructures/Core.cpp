#include "structures/activeStructures/Core.hpp"

#include <iostream>

#include "systems/game_objects/EntityManager.hpp"
#include "systems/game_objects/StructureFactory.hpp"
#include "systems/core/TextureManager.hpp"
#include "entities/Entity.hpp"
#include "map/Map.hpp"

REGISTER_CLASS(Core)

void Core::update()
{
    if (this->active)
    {
        spawnEntities();
    }
}
const int SPAWN_COOLDOWN = 100;
int spawnCooldownCounter = 0;
void Core::spawnEntities()
{
    /*if (spawnCooldownCounter >= SPAWN_COOLDOWN)
    {
        std::vector<SDL_FRect> potentialSpawnTiles = this->getPotentialSpawnTiles((SDL_FRect){-1, -1, 16, 16});
        int i = 0;
        int size = potentialSpawnTiles.size();
        bool finished = false;
        while (i < size && !finished)
        {
            if (!this->map->checkCollisionWithSolidStructure(potentialSpawnTiles[i]))
            {
                this->entityManager->addEntity(this->entityManager->generateDefaultEntity(potentialSpawnTiles[i]));
                finished = true;
            }
            i++;
        }
        spawnCooldownCounter = 0;
    }
    else
    {
        spawnCooldownCounter++;
    }*/
}
std::vector<SDL_FRect> Core::getPotentialSpawnTiles(SDL_FRect entityHitBox)
{
    std::vector<SDL_FRect> potentialSpawnTiles;
    potentialSpawnTiles.push_back((SDL_FRect){this->x - entityHitBox.w, this->y - entityHitBox.h, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x, this->y - entityHitBox.h, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x + entityHitBox.w, this->y - entityHitBox.h, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x + entityHitBox.w, this->y, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x + entityHitBox.w, this->y + entityHitBox.h, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x, this->y + entityHitBox.h, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x - entityHitBox.w, this->y + entityHitBox.h, this->width, this->height});
    potentialSpawnTiles.push_back((SDL_FRect){this->x - entityHitBox.w, this->y, this->width, this->height});
    return potentialSpawnTiles;
}
