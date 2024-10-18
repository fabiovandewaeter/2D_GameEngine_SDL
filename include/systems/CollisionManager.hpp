#ifndef collision_manager_hpp
#define collision_manager_hpp

#include <SDL2/SDL.h>
#include <vector>

class Map;
class EntityManager;
class Entity;

class CollisionManager
{
public:
    CollisionManager();
    ~CollisionManager();

    void init(Map *map, EntityManager *entityManager);
    // returns true if there is a collision
    bool checkCollision(SDL_Rect rectA, SDL_Rect rectB);
    bool checkCollisionFromCoordinates(int x, int y, SDL_Rect rect);
    bool checkCollisionWithSolidStructure(SDL_Rect rect);
    SDL_Rect handleCollisionsFor(Entity *entity, int newPosX, int newPosY);

private:
    Map *map;
    EntityManager *entityManager;
};

#endif