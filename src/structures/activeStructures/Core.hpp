#ifndef core_hpp
#define core_hpp

#include "ActiveStructure.hpp"


class Core : public ActiveStructure
{
public:
    Core(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, Map *map, SDL_Rect hitBox, unsigned int HP);

    void update() override;

private:
    void spawnEntities();
    std::vector<SDL_Rect> getPotentialSpawnTiles(SDL_Rect entityHitBox);
};

#endif