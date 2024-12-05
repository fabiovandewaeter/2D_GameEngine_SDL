#include "map/Map.hpp"

#include <iostream>
#include "map/Chunk.hpp"

#include "systems/Camera.hpp"

Map::Map() {}
Map::~Map()
{
    free();
}

void Map::init(Camera *camera, int tileSize, std::vector<Texture *> *tileTextures, std::vector<Texture *> *staticObjectTextures, std::vector<Texture *> *structureTextures, PerlinNoise *perlinNoise, CollisionManager *collisionManager, SDL_Renderer *renderer)
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
    loadSquareMap(3);
}
void Map::loadSquareMap(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            generateChunk(i * CHUNK_SIZE * this->tileSize, j * CHUNK_SIZE * this->tileSize);
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
    /*
    int size = this->nearbyChunks.size();
    for (int i = 0; i < size; i++)
    {
        this->nearbyChunks[i]->render(this->camera);
    }*/
    // ---------------
    double scale = this->camera->getScale();
    int widthRenderer = 1000;
    int heightRenderer = 1000;
    int cameraWidth = this->camera->getWidth();
    int cameraHeight = this->camera->getHeight();

    SDL_Texture *globalTexture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, widthRenderer, heightRenderer);
    SDL_Texture *previousTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, globalTexture);

    SDL_SetRenderDrawColor(this->renderer, 0, 255, 255, 255);
    SDL_RenderClear(this->renderer);
    int size = this->nearbyChunks.size();
    int minX = 0, minY = 0, width = 0, height = 0;
    for (int i = 0; i < size; i++)
    {
        Chunk *chunk = this->nearbyChunks[i];

        // IL FAUT SUREMENT FAIRE EN FONCTION DU SCALE POUR ENGLOBER TOUS LES CHUNKS VISIBLES
        SDL_Rect renderBox = chunk->getRenderBox();
        // IL FAUT SUREMENT FAIRE EN FONCTION DU SCALE POUR ENGLOBER TOUS LES CHUNKS VISIBLES

        if (camera->isVisible(renderBox))
        {
            SDL_Rect dstRect = {renderBox.x, renderBox.y, renderBox.w, renderBox.h};
            /*if (dstRect.x + dstRect.w > width){width = dstRect.x + dstRect.w;}
            if (dstRect.y + dstRect.h > height){height = dstRect.y + dstRect.h;}
            if (dstRect.x < minX){minX = dstRect.x;}
            if (dstRect.y < minY){minY = dstRect.y;}*/
            int cameraPositionX = dstRect.x;
            int cameraPositionY = dstRect.y;
            int viewCenterX = this->camera->getWidth() / 2;
            int viewCenterY = this->camera->getHeight() / 2;
            dstRect.x = (viewCenterX - cameraPositionX) + (dstRect.x);
            dstRect.y = (viewCenterY - cameraPositionY) + (dstRect.y);

            SDL_RenderCopy(this->renderer, chunk->getCombinedTexte(), NULL, &dstRect);
        }
    }
    int centerX = width / 2;
    int centerY = height / 2;

    SDL_Rect renderBox = {minX, minY, this->camera->getWidth(), this->camera->getHeight()};
    int viewCenterX = widthRenderer / 2;
    int viewCenterY = heightRenderer / 2;
    int cameraPositionX = this->camera->getPositionX();
    int cameraPositionY = this->camera->getPositionY();
    int viewPositionX = (viewCenterX - cameraPositionX * scale) + (renderBox.x * scale);
    int viewPositionY = (viewCenterY - cameraPositionY * scale) + (renderBox.y * scale);

    renderBox.x = 0;
    renderBox.y = 0;
    renderBox.w = widthRenderer;
    renderBox.h = heightRenderer;
    // this->camera->convertInGameToCameraCoordinates(renderBox);
    SDL_SetRenderTarget(this->renderer, previousTarget);

    SDL_Rect finalBox = {0, 0, cameraWidth, cameraHeight};
    finalBox.w *= scale;
    finalBox.h *= scale;
    finalBox.x = ((cameraWidth / 2)) - ((finalBox.w / 2)) - (cameraPositionX*scale);
    finalBox.y = ((cameraHeight / 2)) - ((finalBox.h / 2)) - (cameraPositionY*scale);

    // SDL_RenderCopy(this->renderer, globalTexture, &renderBox, &finalBox);
    SDL_RenderCopy(this->renderer, globalTexture, NULL, &finalBox);
    // SDL_RenderCopy(this->renderer, globalTexture, NULL, &renderBox);

    SDL_DestroyTexture(globalTexture);
    // ---------------
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
