#ifndef entity_hpp
#define entity_hpp

#define VELOCITY_MULTIPLIER 1

#include <SDL2/SDL_render.h>

class Texture;
class State;
class CollisionManager;
class Camera;

class Entity
{
public:
    Entity(Texture *texture, SDL_Rect hitBox, int HP);
    ~Entity();

    void update(CollisionManager *collisionManager);
    bool canMove();
    bool isMoving();
    void move(CollisionManager *collisionManager);
    void render(Camera *camera);
    void onCollision(Entity *);

    void setVelocity(int velX, int velY);
    void setVelocityX(int velocityX);
    void setVelocityY(int velocityY);
    void hit(int damage);

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
};

#endif