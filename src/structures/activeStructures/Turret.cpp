#include "structures/activeStructures/Turret.hpp"

#include <iostream>
#include "actions/Action.hpp"
#include "actions/patterns/SquarePattern.hpp"
#include "actions/effects/DamageEffect.hpp"

class Effects;

Turret::Turret(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP) : ActiveStructure(texture, collisionManager, entityManager, hitBox, HP, true)
{
    this->actions.push_back(new Action(new SquarePattern(this->entityManager, 30), std::vector<Effect *>{new DamageEffect(10)}, 60));
}

void Turret::update()
{
    if (this->active)
    {
        int size = this->actions.size();
        for (int i = 0; i < size; i++)
        {
            this->actions[i]->apply(this->hitBox.x + (this->hitBox.w / 2), this->hitBox.y + (this->hitBox.h / 2));
        }
    }
}