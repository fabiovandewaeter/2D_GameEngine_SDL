#include "actions/effects/HealEffect.hpp"

#include "entities/Entity.hpp"

HealEffect::HealEffect(int hp) : Effect() { this->hp = hp; }
HealEffect::~HealEffect() {}

void HealEffect::applyEffect(Entity *entity)
{
    entity->hit(hp);
}
