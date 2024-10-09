#ifndef square_pattern_hpp
#define square_pattern_hpp

#include "Pattern.hpp"

#include <vector>

class SquarePattern : public Pattern
{
public:
    SquarePattern(EntityManager *entityManager, int radius);
    ~SquarePattern();

    std::vector<Entity *> affectedEntities(int x, int y) override;

private:
};

#endif