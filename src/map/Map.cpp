#include "Map.hpp"

#include "Chunk.hpp"
#include <iostream>

Map::Map() {}
Map::~Map()
{
    for (auto &pair : this->allChunks)
    {
        delete pair.second;
    }
    this->nearbyChunks.clear();
    this->allChunks.clear();
}

void Map::init(Camera *camera, std::vector<Texture *> *tileTextures, std::vector<Texture *> *staticObjectTextures, std::vector<Texture *> *structureTextures, PerlinNoise *perlinNoise, CollisionManager *collisionManager)
{
    this->camera = camera;
    this->tileTextures = tileTextures;
    this->staticObjectTextures = staticObjectTextures;
    this->structureTextures = structureTextures;
    this->perlinNoise = perlinNoise;
    this->collisionManager = collisionManager;
    loadChunks();
}

void Map::loadChunks()
{
    loadSquareMap(10);
}
void Map::loadSquareMap(int size)
{
    for (int i = -size / 2; i < size / 2; i++)
    {
        for (int j = -size / 2; j < size / 2; j++)
        {
            generateChunk(TILE_SIZE * CHUNK_SIZE * i, j * TILE_SIZE * CHUNK_SIZE);
        }
    }
}

void Map::generateChunk(int positionX, int positionY)
{
    Chunk *newChunk = new Chunk(positionX, positionY, TILE_SIZE, this, this->tileTextures, this->staticObjectTextures, this->structureTextures, this->perlinNoise, this->collisionManager);
    this->nearbyChunks.push_back(newChunk);
    int i = positionX, j = positionY;
    convertToChunkCoordinates(i, j);
    std::string coordinates = std::to_string(i) + "," + std::to_string(j);
    this->allChunks[coordinates] = newChunk;
}

void Map::render()
{
    int size = this->nearbyChunks.size();
    for (int i = 0; i < size; i++)
    {
        this->nearbyChunks[i]->render(this->camera);
    }
}
void Map::update()
{
    int size = this->nearbyChunks.size();
    for (int i = 0; i < size; i++)
    {
        this->nearbyChunks[i]->update();
    }
}
void Map::free()
{
    printf("FAIRE LE FREE DE MAP\n");
}

void Map::convertToChunkCoordinates(int &x, int &y)
{
    x = std::floor(static_cast<float>(x) / (CHUNK_SIZE * TILE_SIZE));
    y = std::floor(static_cast<float>(y) / (CHUNK_SIZE * TILE_SIZE));
}

// returns true if the chunk exist
bool Map::isChunkGenerated(int x, int y)
{
    return true;
    int i = x, j = y;
    convertToChunkCoordinates(i, j);
    std::string coordinates = std::to_string(i) + "," + std::to_string(j);

    if (this->allChunks.find(coordinates) == this->allChunks.end())
    {
        return false;
    }
    return true;
}

// returns the chunk that contains the coordinates ; generates the chunk if it is not already done
Chunk *Map::getChunk(int x, int y)
{
    int i = x, j = y;
    convertToChunkCoordinates(i, j);
    std::string coordinates = std::to_string(i) + "," + std::to_string(j);

    if (this->allChunks.find(coordinates) == this->allChunks.end())
    {
        generateChunk(i * TILE_SIZE * CHUNK_SIZE, j * TILE_SIZE * CHUNK_SIZE);
        std::cout << "Chunk generated at (" << coordinates << ") | Total: " << this->allChunks.size() << std::endl;
    }
    return this->allChunks[coordinates];
}
int Map::getTileSize()
{
    return TILE_SIZE;
}
int Map::getChunkSize()
{
    return CHUNK_SIZE;
}