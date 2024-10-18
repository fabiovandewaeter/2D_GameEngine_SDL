#include "actions/effects/DamageEffect.hpp"

#include "iostream"
#include "entities/Entity.hpp"

DamageEffect::DamageEffect(int damage) : Effect() { this->damage = damage; }
DamageEffect::~DamageEffect() {}

void DamageEffect::applyEffect(Entity *entity)
{
    std::cout << entity->getHP() << std::endl;
    entity->hit(damage);
}
