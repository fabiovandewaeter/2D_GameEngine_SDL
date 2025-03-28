#ifdef PROFILER
#include "tracy_profiler/tracy/Tracy.hpp"
#endif

#include <limits>
#include <cmath>

#include "map/Chunk.hpp"

#include "map/Tile.hpp"
#include "systems/algorithms/PerlinNoise.hpp"
#include "systems/core/Camera.hpp"
#include "systems/game_objects/ItemFactory.hpp"
#include "systems/core/TextureManager.hpp"
#include "structures/passiveStructures/Wall.hpp"
#include "structures/IUpdatable.hpp"
#include "systems/utils/Constants.hpp"
#include "entities/Entity.hpp"

#include "structures/passiveStructures/Tree.hpp"

Chunk::Chunk(int positionX, int positionY, Map *map, PerlinNoise *perlinNoise, CollisionManager *collisionManager) : positionX(positionX), positionY(positionY), width(CHUNK_TILE_SIZE), height(CHUNK_TILE_SIZE), map(map), perlinNoise(perlinNoise)
{
    loadTiles();
    loadUpdatableStructures();
    loadOtherStructures();
}

Chunk::~Chunk()
{
    for (int i = 0; i < CHUNK_TILE_SIZE * CHUNK_TILE_SIZE; i++)
    {
        delete this->allTiles[i];
    }
    for (auto &pair : this->updatableStructures)
    {
        delete pair.second;
    }
    for (auto &pair : this->otherStructures)
    {
        delete pair.second;
    }
}

void Chunk::loadTiles()
{
    loadTilesWithPerlinNoise();
}

void Chunk::loadTilesDefault()
{
    for (int i = 0; i < CHUNK_TILE_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_TILE_SIZE; j++)
        {
            this->allTiles[CHUNK_TILE_SIZE * i + j] = new Tile("grass_0", i + this->positionX, j + this->positionY);
        }
    }
}

void Chunk::loadTilesWithPerlinNoise()
{
    for (int i = 0; i < CHUNK_TILE_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_TILE_SIZE; j++)
        {
            float positionX = i + this->positionX;
            float positionY = j + this->positionY;
            double res = this->perlinNoise->perlin2d(positionX, positionY, 0.05f, 1);
            int textureIndex = 0;
            int numberOfTileTextures = 4;
            if (res < 1.0 / numberOfTileTextures)
            {
                textureIndex = 0;
            }
            else if (res < 2 * (1.0 / numberOfTileTextures))
            {
                textureIndex = 1;
            }
            else if (res < 3 * (1.0 / numberOfTileTextures))
            {
                textureIndex = 2;
            }
            else if (res < 4 * (1.0 / numberOfTileTextures))
            {
                textureIndex = 3;
            }
            std::string textureName = "grass_" + std::to_string(textureIndex);
            allTiles[CHUNK_TILE_SIZE * i + j] = new Tile(textureName, positionX, positionY);
        }
    }
}

void Chunk::loadUpdatableStructures() {}
void Chunk::loadOtherStructures() {}

void Chunk::render(Camera *camera)
{
    SDL_FRect renderBox = {positionX, positionY, width, height};
    SDL_Rect newRenderBox = camera->convertInGameToCameraCoordinates(renderBox);
    if (camera->isVisibleOnScreen(newRenderBox))
    {
        for (int i = 0; i < CHUNK_TILE_SIZE * CHUNK_TILE_SIZE; i++)
        {
            allTiles[i]->render(camera);
        }
    }
    for (auto &pair : updatableStructures)
    {
        pair.second->render(camera);
    }
    for (auto &pair : otherStructures)
    {
        pair.second->render(camera);
    }
}

void Chunk::update()
{
    std::vector<std::pair<int, int>> updatableStructuresToRemove;
    std::vector<std::pair<int, int>> otherStructuresToRemove;
    for (auto &[coords, structure] : updatableStructures)
    {
        structure->update();
        if (structure->isDestroyed())
        {
            updatableStructuresToRemove.push_back(coords);
        }
    }
    // NEED CHANGE TO NOT CHECK EVERY TICK IF ALL WALLS ARE DESTROYED
    for (auto &[coords, structure] : otherStructures)
    {
        if (structure->isDestroyed())
        {
            otherStructuresToRemove.push_back(coords);
        }
    }
    for (const auto &coords : updatableStructuresToRemove)
    {
        delete updatableStructures[coords];
        updatableStructures.erase(coords);
    }
    for (const auto &coords : otherStructuresToRemove)
    {
        delete otherStructures[coords];
        otherStructures.erase(coords);
    }
}

std::pair<int, int> Chunk::convertToLocalTileCoordinates(float positionX, float positionY)
{
    int newX = static_cast<int>(std::floor(positionX)) % CHUNK_TILE_SIZE;
    int newY = static_cast<int>(std::floor(positionY)) % CHUNK_TILE_SIZE;
    if (positionX < 0)
    {
        newX = CHUNK_TILE_SIZE + newX;
    }
    if (positionY < 0)
    {
        newY = CHUNK_TILE_SIZE + newY;
    }
    std::pair<int, int> res = {newX, newY};
    return res;
}

// returns the tile that contains the coordinates
Tile *Chunk::getTile(float positionX, float positionY)
{
    std::pair<int, int> newCoordinates = convertToLocalTileCoordinates(positionX, positionY);
    return allTiles[CHUNK_TILE_SIZE * newCoordinates.first + newCoordinates.second];
}

Structure *Chunk::getStructure(float positionX, float positionY)
{
    std::pair<int, int> newCoordinates = convertToLocalTileCoordinates(positionX, positionY);
    auto it = updatableStructures.find(newCoordinates);
    if (it != updatableStructures.end())
    {
        return it->second;
    }
    else
    {
        it = otherStructures.find(newCoordinates);
        if (it != otherStructures.end())
        {
            return it->second;
        }
    }
    return nullptr;
}

bool Chunk::isStructure(float positionX, float positionY)
{
    std::pair<int, int> newCoordinates = convertToLocalTileCoordinates(positionX, positionY);
    auto it = updatableStructures.find(newCoordinates);
    if (it != updatableStructures.end())
    {
        return true;
    }
    else
    {
        it = otherStructures.find(newCoordinates);
        if (it != otherStructures.end())
        {
            return true;
        }
    }
    return false;
}

void Chunk::addStructure(Structure *structure, float positionX, float positionY)
{
    if (!isStructure(positionX, positionY))
    {
        std::pair<int, int> newCoordinates = convertToLocalTileCoordinates(positionX, positionY);
        SDL_FRect box = {std::floor(positionX), std::floor(positionY), 1, 1}; // floor to make sure the coordinates are based on the grid
        structure->setHitBox(box);
        if (dynamic_cast<IUpdatable *>(structure))
        {
            updatableStructures[newCoordinates] = structure;
        }
        else
        {
            otherStructures[newCoordinates] = structure;
        }
    }
}

void Chunk::destroyStructure(float positionX, float positionY)
{
    if (isStructure(positionX, positionY))
    {
        std::pair<int, int> newCoordinates = convertToLocalTileCoordinates(positionX, positionY);
        Structure *structure = getStructure(newCoordinates.first, newCoordinates.second);
        if (IUpdatable *updatable = dynamic_cast<IUpdatable *>(structure))
        {
            updatableStructures.erase(newCoordinates);
        }
        else
        {
            otherStructures.erase(newCoordinates);
        }
        std::cout << "Chunk::destroyStructure() : structure destroyed at (" << newCoordinates.first << "," << newCoordinates.second << ")" << std::endl;
        structure->destroy();
    }
}

Structure *Chunk::breakStructure(float positionX, float positionY)
{
    if (isStructure(positionX, positionY))
    {
        std::pair<int, int> newCoordinates = convertToLocalTileCoordinates(positionX, positionY);
        Structure *structure = getStructure(newCoordinates.first, newCoordinates.second);
        if (IUpdatable *updatable = dynamic_cast<IUpdatable *>(structure))
        {
            updatableStructures.erase(newCoordinates);
        }
        else
        {
            otherStructures.erase(newCoordinates);
        }
        std::cout << "Chunk::breakStructure() : structure broken at (" << newCoordinates.first << "," << newCoordinates.second << ")" << std::endl;
        return structure;
    }
    return nullptr;
}

void Chunk::setFaction(Faction *faction) { this->faction = faction; }

Structure *Chunk::findClosestStructure(const std::string structureClassName, const Entity *entity)
{
    float minDistance = std::numeric_limits<float>::max();
    Structure *closestStructure = nullptr;
    // Parcourt les structures "updatable"
    for (auto &entry : updatableStructures)
    {
        Structure *structure = entry.second;
        if (structure && structure->getClassName() == structureClassName)
        {
            float dx = structure->getPositionX() - entity->getPositionX();
            float dy = structure->getPositionY() - entity->getPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestStructure = structure;
            }
        }
    }
    // Parcourt les autres structures
    for (auto &entry : otherStructures)
    {
        Structure *structure = entry.second;
        if (structure && structure->getClassName() == structureClassName)
        {
            float dx = structure->getPositionX() - entity->getPositionX();
            float dy = structure->getPositionY() - entity->getPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestStructure = structure;
            }
        }
    }
    return closestStructure;
}
