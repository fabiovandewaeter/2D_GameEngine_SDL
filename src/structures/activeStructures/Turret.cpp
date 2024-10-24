#include "structures/activeStructures/Turret.hpp"

#include <iostream>
#include "actions/Action.hpp"
#include "actions/patterns/SquarePattern.hpp"
#include "actions/effects/DamageEffect.hpp"

class Effects;

Turret::Turret(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP, Faction *faction) : ActiveStructure(texture, collisionManager, entityManager, hitBox, HP, true, faction)
{
    std::vector<Effect *> effects;
    effects.push_back(new DamageEffect(10));
    this->actions.push_back(new Action(new SquarePattern(this->entityManager, 30), effects, 60));
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