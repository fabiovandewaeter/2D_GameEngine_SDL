#ifndef structure_hpp
#define structure_hpp

#include <SDL2/SDL_rect.h>
#include <iostream>
#include <string>

#include "systems/core/TextureManager.hpp"
#include "map/Tile.hpp"
#include "Texture.hpp"

class TextureManager;
class Camera;
class Entity;
class CollisionManager;
class Map;
class Player;
class TickManager;

class Structure
{
public:
    Structure() : Structure(nullptr, -1, -1, 0, false, nullptr, nullptr) {}
    Structure(Texture *texture, int x, int y, Player *placedBy, TickManager *tickManager) : Structure(texture, x, y, 0, true, placedBy, tickManager) {}
    Structure(Texture *texture, int x, int y, int HP, bool solid, Player *placedBy, TickManager *tickManager) : texture(texture), hitBox({x, y, getTileSize(), getTileSize()}), HP(HP), solid(solid), destroyed(false) {}
    ~Structure();

    void update();
    void destroy();
    void render(Camera *camera);
    void onCollision(Entity *entity);
    virtual void onLeftClick();
    virtual void onRightClick();

    SDL_Rect getHitBox();
    int getHP();
    bool isSolid();
    bool isDestroyed();
    void setX(int x);
    void setY(int y);
    void setHitBox(SDL_Rect hitBox);
    void setTexture(Texture *texture);
    static std::string getName() { return "Structure"; };
    static int getTexture()
    {
        std::cout << "ERROR: default Structure::getTexture() should not be used" << std::endl;
        return -1;
    };

protected:
    Texture *texture;
    SDL_Rect hitBox;
    bool solid;
    unsigned int HP;
    bool destroyed;

    int getTileSize();
};

#endif
