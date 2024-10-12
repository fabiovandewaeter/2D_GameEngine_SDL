#ifndef passive_structure_hpp
#define passive_structure_hpp

#include "../Structure.hpp"

class PassiveStructure : public Structure
{
public:
    PassiveStructure(Texture *texture, SDL_Rect hitBox, unsigned int HP);

    void onRightClick() override;

private:
};

#endif