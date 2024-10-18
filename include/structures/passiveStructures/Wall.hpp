#ifndef wall_hpp
#define wall_hpp

#include "PassiveStructure.hpp"

class Wall : public PassiveStructure
{
public:
    Wall(Texture *texture, SDL_Rect hitBox, unsigned int HP);

private:
};

#endif