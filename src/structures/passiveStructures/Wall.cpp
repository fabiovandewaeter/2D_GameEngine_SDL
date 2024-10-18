#include "structures/passiveStructures/Wall.hpp"

Wall::Wall(Texture *texture, SDL_Rect hitBox, unsigned int HP) : PassiveStructure(texture, hitBox, HP) {}