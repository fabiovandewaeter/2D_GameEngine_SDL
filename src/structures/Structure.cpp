#include "structures/Structure.hpp"

#include "systems/core/Camera.hpp"
#include "Texture.hpp"
#include "structures/materials/Material.hpp"

Structure::~Structure() {}

void Structure::render(Camera *camera)
{
    SDL_Rect renderBox = this->hitBox;
    camera->convertInGameToCameraCoordinates(renderBox);
    if (camera->isVisible(renderBox))
    {
        this->texture->render(renderBox);
    }
}

void Structure::update() {}
void Structure::destroy() { delete this; }
void Structure::onCollision(Entity *entity) { std::cout << "Structure#onCollision()" << std::endl; }
void Structure::onLeftClick() { std::cout << "fait rien LEFT" << std::endl; }
void Structure::onRightClick() { std::cout << "fait rien RIGHT" << std::endl; }

SDL_Rect Structure::getHitBox() { return this->hitBox; }
int Structure::getHP() { return this->material->getHP(); }
bool Structure::isSolid() { return this->solid; }
bool Structure::isDestroyed() { return this->destroyed; }
void Structure::setHitBox(SDL_Rect hitBox) { this->hitBox = hitBox; }
int Structure::getTileSize() { return Tile::getTileSize(); }
