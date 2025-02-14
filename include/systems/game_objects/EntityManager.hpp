#ifndef entity_manager_hpp
#define entity_manager_hpp

#include <SDL2/SDL.h>
#include <vector>

class Map;
class Camera;
class Entity;
class Player;

class EntityManager
{
public:
    EntityManager(Map *map) : map(map) {};
    ~EntityManager();

    void loadEntities();
    void update();
    void render(Camera *camera);

    void addPlayer(Player *player);
    void addEntity(Entity *entity);
    bool checkCollision(SDL_Rect rectA, SDL_Rect rectB);
    std::vector<Entity *> getEntities();
    std::vector<Entity *> getPotentialEntities(Entity *entity);
    std::vector<Entity *> getEntitiesInArea(SDL_Rect area);
    Player *getPlayer();

private:
    Map *map;
    std::vector<Entity *> entities;
    Player *player;
};

#endif
