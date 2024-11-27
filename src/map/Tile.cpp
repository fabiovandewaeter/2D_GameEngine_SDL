#include "map/Tile.hpp"

#include "systems/Camera.hpp"
#include "Texture.hpp"

Tile::Tile(Texture *texture, int x, int y)
{
    this->texture = texture;
    this->x = x;
    this->y = y;
}
Tile::~Tile() {}

void Tile::render(Camera *camera)
{
    SDL_Rect dstBox = {this->x, this->y, TILE_SIZE, TILE_SIZE};
    // 1px paddings to avoid gap when zoom out
    camera->convertInGameToCameraCoordinates(dstBox);
    dstBox.x -= 1;
    dstBox.y -= 1;
    dstBox.w += 2;
    dstBox.h += 2;
    if (camera->isVisible(dstBox))
    {
        // only the texture on the center and avoid all padding
        SDL_Rect srcBox;
        srcBox.x = this->texture->getCenterX() - TEXTURE_DEFAULT_SIZE/ 2;
        srcBox.y = this->texture->getCenterY() - TEXTURE_DEFAULT_SIZE/ 2;
        srcBox.w = TEXTURE_DEFAULT_SIZE;
        srcBox.h = TEXTURE_DEFAULT_SIZE;

        // 1px paddings to avoid gap when zoom out
        /*srcBox.x -= 1;
        srcBox.y -= 1;
        srcBox.w += 2;
        srcBox.h += 2;*/
        this->texture->render(srcBox, dstBox);
    }
}
    void Tile::render(SDL_Renderer *renderer, SDL_Rect srcBox, SDL_Rect dstBox){
        this->texture->render(renderer, srcBox, dstBox);
    }

int Tile::getCenterX() { return TILE_SIZE / 2; }
int Tile::getCenterY() { return TILE_SIZE / 2; }
int Tile::getTextureId() { return this->texture->getId(); }
int Tile::getTileSize() { return TILE_SIZE; }