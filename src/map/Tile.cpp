#include "map/Tile.hpp"

#include "systems/Camera.hpp"
#include "Texture.hpp"

Tile::Tile(Texture *texture, SDL_Rect box)
{
    this->texture = texture;
    this->box = box;
}
Tile::~Tile() {}

void Tile::render(Camera *camera)
{
    SDL_Rect renderBox = this->box;
    camera->convertInGameToCameraCoordinates(renderBox);
    if (camera->isVisible(renderBox))
    {
        this->texture->render(renderBox);
    }
}

int Tile::getCenterX() { return this->box.w / 2; }
int Tile::getCenterY() { return this->box.h / 2; }
int Tile::getTextureId() { return this->texture->getId(); }
SDL_Rect Tile::getBox() { return this->box; }