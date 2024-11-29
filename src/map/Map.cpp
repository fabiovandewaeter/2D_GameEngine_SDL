#include "map/Map.hpp"

#include <iostream>
#include "map/Chunk.hpp"

#include "systems/Camera.hpp"

Map::Map() {}
Map::~Map()
{
    free();
}

void Map::init(Camera *camera, int tileSize, std::vector<Texture *> *tileTextures, std::vector<Texture *> *staticObjectTextures, std::vector<Texture *> *structureTextures, PerlinNoise *perlinNoise, CollisionManager *collisionManager, SDL_Renderer * renderer)
{
    this->camera = camera;
    this->tileSize = tileSize;
    this->tileTextures = tileTextures;
    this->staticObjectTextures = staticObjectTextures;
    this->structureTextures = structureTextures;
    this->perlinNoise = perlinNoise;
    this->collisionManager = collisionManager;
    this->renderer = renderer;
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
            generateChunk(this->tileSize * CHUNK_SIZE * i, j * this->tileSize * CHUNK_SIZE);
        }
    }
}

void Map::generateChunk(int positionX, int positionY)
{
    Chunk *newChunk = new Chunk(positionX, positionY, this->tileSize, this, this->tileTextures, this->staticObjectTextures, this->structureTextures, this->perlinNoise, this->collisionManager);
    this->nearbyChunks.push_back(newChunk);
    int i = positionX, j = positionY;
    convertToChunkCoordinates(i, j);
    std::string coordinates = std::to_string(i) + "," + std::to_string(j);
    this->allChunks[coordinates] = newChunk;
}


void Map::render()
{
    /*int size = this->nearbyChunks.size();
    for (int i = 0; i < size; i++)
    {
        this->nearbyChunks[i]->render(this->camera);
    }*/
    SDL_Texture *globalTexture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->camera->getWidth(), this->camera->getHeight());
    SDL_Texture *previousTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, globalTexture);

  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);  // Couleur noire, par exemple
    SDL_RenderClear(this->renderer);
    int size = this->nearbyChunks.size();
    for (int i = 0; i < size; i++)
    {
        Chunk *chunk = this->nearbyChunks[i];
        SDL_Rect renderBox = chunk->getRenderBox();
        this->camera->convertInGameToCameraCoordinates(renderBox);

        // S'assurer que le chunk est visible avant de le rendre
        if (camera->isVisible(renderBox))
        {
            //SDL_Rect dstRect = {i * this->tileSize, j * this->tileSize, this->tileSize, this->tileSize};
            SDL_RenderCopy(this->renderer, this->nearbyChunks[i]->getCombinedTexte(), NULL, &renderBox);
        }
    }
    SDL_Rect renderBox = {0, 0, this->camera->getWidth(), this->camera->getHeight()};
    SDL_SetRenderTarget(this->renderer, previousTarget);

    SDL_RenderCopy(this->renderer, globalTexture, NULL, &renderBox);

    SDL_DestroyTexture(globalTexture);
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
    for (auto &pair : this->allChunks)
    {
        delete pair.second;
    }
    this->allChunks.clear();
    this->nearbyChunks.clear();
}

void Map::convertToChunkCoordinates(int &x, int &y)
{
    x = std::floor(static_cast<float>(x) / (CHUNK_SIZE * this->tileSize));
    y = std::floor(static_cast<float>(y) / (CHUNK_SIZE * this->tileSize));
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
        generateChunk(i * this->tileSize * CHUNK_SIZE, j * this->tileSize * CHUNK_SIZE);
        std::cout << "Chunk generated at (" << coordinates << ") | Total: " << this->allChunks.size() << std::endl;
    }
    return this->allChunks[coordinates];
}
int Map::getTileSize()
{
    return this->tileSize;
}
int Map::getChunkSize()
{
    return CHUNK_SIZE;
}
