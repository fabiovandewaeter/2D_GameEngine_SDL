#ifndef pattern_hpp
#define pattern_hpp

#include <vector>

class Entity;
class EntityManager;

class Pattern
{
public:
    Pattern(EntityManager *entityManager, int radius);
    ~Pattern();

    virtual std::vector<Entity *> getAffectedEntities(int x, int y) = 0;

protected:
    EntityManager *entityManager;
    int radius;
};

#endif