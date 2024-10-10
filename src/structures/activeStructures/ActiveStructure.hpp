#ifndef active_structure_hpp
#define active_structure_hpp

#include "../Structure.hpp"
#include <vector>

class EntityManager;
class TextureManager;
class Map;

class ActiveStructure : public Structure
{
public:
    ActiveStructure(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, Map *map, SDL_Rect hitBox, unsigned int HP, bool solid);
    ~ActiveStructure();

    virtual void spawnEntities();
    std::vector<SDL_Rect> getPotentialSpawnTiles();
    void onLeftClick() override;

protected:
    CollisionManager *collisionManager;
    EntityManager *entityManager;
    bool active;
};

#endif