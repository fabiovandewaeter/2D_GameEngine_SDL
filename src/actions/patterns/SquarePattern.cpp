#include "actions/patterns/SquarePattern.hpp"

#include <iostream>
#include "systems/EntityManager.hpp"

SquarePattern::SquarePattern(EntityManager *entityManager, int radius) : Pattern(entityManager, radius) {}
SquarePattern::~SquarePattern() {}

std::vector<Entity *> SquarePattern::getAffectedEntities(int x, int y)
{
    return this->entityManager->getEntitiesInArea((SDL_Rect){x - (this->radius / 2), y - (this->radius / 2), this->radius, this->radius});
}