#include "PassiveStructure.hpp"

#include "../../map/Map.hpp"
#include "../../map/Chunk.hpp"

PassiveStructure::PassiveStructure(Texture *texture, SDL_Rect hitBox, unsigned int HP) : Structure(texture, hitBox, HP, true) {}

void PassiveStructure::onRightClick() { this->destroyed = true; }