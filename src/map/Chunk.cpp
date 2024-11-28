#include "map/Chunk.hpp"

#include "map/Tile.hpp"
#include "systems/PerlinNoise.hpp"
#include "systems/Camera.hpp"
#include "structures/passiveStructures/Wall.hpp"

#include "Texture.hpp"

Chunk::Chunk(int positionX, int positionY, int tileSize, Map *map, std::vector<Texture *> *tileTextures, std::vector<Texture *> *passiveStructureTextures, std::vector<Texture *> *activeStructureTextures, PerlinNoise *perlinNoise, CollisionManager *collisionManager)
{
    this->positionX = positionX;
    this->positionY = positionY;
    this->tileSize = tileSize;
    this->map = map;
    this->tileTextures = tileTextures;
    this->passiveStructureTextures = passiveStructureTextures;
    this->activeStructureTextures = activeStructureTextures;
    this->box = (SDL_Rect){positionX, positionY, tileSize * SIZE, tileSize * SIZE};
    this->perlinNoise = perlinNoise;
    loadTiles();
    loadPassiveStructures();
    loadActiveStructures();
}
Chunk::~Chunk()
{
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        delete this->allTiles[i];
    }
    for (auto &pair : this->allStructures)
    {
        delete pair.second;
    }
    if (this->combinedTexture != nullptr)
    {
        SDL_DestroyTexture(this->combinedTexture);
    }
}

void Chunk::loadTiles()
{
    loadTilesWithPerlinNoise();
}
void Chunk::loadTilesDefault()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            this->allTiles[SIZE * i + j] = new Tile((*this->tileTextures)[0], i * this->tileSize + this->box.x, j * this->tileSize + this->box.y);
        }
    }
}
void Chunk::loadTilesWithPerlinNoise()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int x = i * this->tileSize + this->box.x;
            int y = j * this->tileSize + this->box.y;
            double res = this->perlinNoise->perlin2d(x, y, 0.001f, 1);
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
            this->allTiles[SIZE * i + j] = new Tile((*this->tileTextures)[textureIndex], x, y);
        }
    }
    this->renderer = (*this->tileTextures)[0]->getRenderer();
    generateCombinedTexture(this->renderer);
}
void Chunk::loadPassiveStructures() {}
void Chunk::loadActiveStructures() {}

void Chunk::generateCombinedTexture(SDL_Renderer *renderer)
{
    if (this->combinedTexture != nullptr)
    {
        SDL_DestroyTexture(this->combinedTexture); // Libérer la texture précédente si elle existe
    }

    // Créer une texture cible
    this->combinedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->box.w, this->box.h);
    if (this->combinedTexture == nullptr)
    {
        std::cerr << "Erreur lors de la création de la texture combinée : " << SDL_GetError() << std::endl;
        return;
    }

    if (SDL_SetTextureBlendMode(this->combinedTexture, SDL_BLENDMODE_BLEND) != 0)
    {
        std::cout << "pas bon" << std::endl;
    }

    // Sauvegarder le target actuel et définir la texture combinée comme target
    SDL_Texture *previousTarget = SDL_GetRenderTarget(renderer);
    // SDL_SetRenderTarget(renderer, this->combinedTexture);
    if (SDL_SetRenderTarget(renderer, this->combinedTexture) != 0)
    {
        std::cerr << "Erreur lors de la définition de la texture combinée comme target : " << SDL_GetError() << std::endl;
        return;
    }

    // Dessiner toutes les tiles sur la texture combinée
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            SDL_Rect dstRect = {i * this->tileSize, j * this->tileSize, this->tileSize, this->tileSize};
            this->allTiles[SIZE * i + j]->render(renderer, (SDL_Rect){0, 0, 0, 0}, dstRect);
        }
    }

    // Restaurer le target précédent
    SDL_SetRenderTarget(renderer, previousTarget);
}

void Chunk::render(Camera *camera)
{
    SDL_Rect renderBox = this->box;
    camera->convertInGameToCameraCoordinates(renderBox);

    if (camera->isVisible(renderBox))
    {
        if (this->combinedTexture != nullptr)
        {
            int cameraPositionX = camera->getPositionX();
            int cameraPositionY = camera->getPositionY();
            int viewCenterX = camera->getWidth() / 2;
            int viewCenterY = camera->getHeight() / 2;
            SDL_Rect dstRect = this->box;
            // Position ajustée avec arrondi pour éviter les flottants
            dstRect.x = static_cast<int>(std::round(
                (viewCenterX - cameraPositionX * camera->getScale()) + (dstRect.x * camera->getScale())));
            dstRect.y = static_cast<int>(std::round(
                (viewCenterY - cameraPositionY * camera->getScale()) + (dstRect.y * camera->getScale())));

            // Dimensions ajustées
            dstRect.w = static_cast<int>(dstRect.w * camera->getScale());
            dstRect.h = static_cast<int>(dstRect.h * camera->getScale());

            // Désactiver l'interpolation

            // Rendu
            SDL_RenderCopy(this->renderer, this->combinedTexture, NULL, &dstRect);
        }
        else
        {
            for (int i = 0; i < SIZE * SIZE; i++)
            {
                this->allTiles[i]->render(camera);
            }
        }
    }
    for (auto &pair : this->allStructures)
    {
        pair.second->render(camera);
    }
}

void Chunk::update()
{
    std::vector<std::string> toRemove;
    for (auto &[coords, structure] : allStructures)
    {
        structure->update();
        if (structure->isDestroyed())
        {
            toRemove.push_back(coords);
        }
    }
    for (const auto &coords : toRemove)
    {
        delete this->allStructures[coords];
        this->allStructures.erase(coords);
    }
}

void Chunk::convertToTileCoordinates(int &x, int &y)
{
    x = static_cast<int>(std::floor(static_cast<float>(x) / this->tileSize)) % SIZE;
    y = static_cast<int>(std::floor(static_cast<float>(y) / this->tileSize)) % SIZE;
    if (x < 0)
    {
        x = SIZE + x;
    }
    if (y < 0)
    {
        y = SIZE + y;
    }
}
// returns the tile that contains the coordinates
Tile *Chunk::getTile(int x, int y)
{
    convertToTileCoordinates(x, y);
    return this->allTiles[SIZE * x + y];
}
Structure *Chunk::getStructure(int x, int y)
{
    convertToTileCoordinates(x, y);
    std::string coordinates = std::to_string(x) + "," + std::to_string(y);
    return this->allStructures[coordinates];
}
bool Chunk::isStructure(int x, int y)
{
    convertToTileCoordinates(x, y);
    std::string coordinates = std::to_string(x) + "," + std::to_string(y);
    if (this->allStructures.find(coordinates) == this->allStructures.end())
    {
        return false;
    }
    return true;
}
void Chunk::addStructure(Structure *structure)
{
    SDL_Rect hitBox = structure->getHitBox();
    int x = hitBox.x;
    int y = hitBox.y;
    if (!isStructure(x, y))
    {
        convertToTileCoordinates(x, y);
        SDL_Rect box = {x * this->tileSize + this->box.x, y * this->tileSize + this->box.y, this->tileSize, this->tileSize};
        structure->setHitBox(box);
        std::string coordinates = std::to_string(x) + "," + std::to_string(y);
        this->allStructures[coordinates] = structure;
    }
}
void Chunk::addWall(int x, int y)
{
    addStructure(new Wall((*this->passiveStructureTextures)[1], (SDL_Rect){x, y, this->tileSize, this->tileSize}, 100));
}
void Chunk::destroyStructure(int x, int y)
{
    printf("2\n");
    if (isStructure(x, y))
    {
        printf("3\n");
        convertToTileCoordinates(x, y);
        std::string coordinates = std::to_string(x) + "," + std::to_string(y);
        this->allStructures[coordinates]->destroy();
        this->allStructures.erase(coordinates);
    }
}
void Chunk::setFaction(Faction *faction) { this->faction = faction; }
