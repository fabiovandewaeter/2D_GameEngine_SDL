#ifndef entity_hpp
#define entity_hpp

#define VELOCITY_MULTIPLIER 1

#include <SDL2/SDL_render.h>

class Texture;
class State;
class Camera;
class Faction;
class Map;

class Entity
{
public:
    Entity(Texture *texture, SDL_Rect hitBox, int HP);
    ~Entity();

    void update(Map *map);
    bool canMove();
    bool isMoving();
    void move(Map *map);
    void render(Camera *camera);
    void kill();
    void onCollision(Entity *);
    void hit(int damage);
    virtual void onLeftClick();
    virtual void onRightClick();

    void setVelocity(int velX, int velY);
    void setVelocityX(int velocityX);
    void setVelocityY(int velocityY);
    void setFaction(Faction *faction);

    int getPositionX();
    int getPositionY();
    int getCenterX();
    int getCenterY();
    SDL_Rect getHitBox();
    int getHP();

protected:
    Texture *texture;
    int velX, velY;
    SDL_Rect hitBox;
    State *state;
    int HP;
    Faction *faction;
};

#endif
