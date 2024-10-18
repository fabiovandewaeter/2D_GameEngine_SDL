#include "actions/patterns/Pattern.hpp"

Pattern::Pattern(EntityManager *entityManager, int radius)
{
    this->entityManager = entityManager;
    this->radius = radius;
}
Pattern::~Pattern() {}