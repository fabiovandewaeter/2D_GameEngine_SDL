#ifndef turret_hpp
#define turret_hpp

#include "ActiveStructure.hpp"


class Turret : public ActiveStructure
{
public:
    Turret(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP);

    void update() override;
};

#endif