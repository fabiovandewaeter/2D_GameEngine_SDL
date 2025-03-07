#ifndef break_structure_action_hpp
#define break_structure_action_hpp

#include "Action.hpp"

#include <vector>
#include "SDL2/SDL_rect.h"

class BreakStructureAction : public Action
{
public:
    BreakStructureAction(float goalX, float goalY, Entity *entity) : Action(entity), goalX(goalX), goalY(goalY), completed(false) {}
    void execute() override;
    bool isCompleted() override;

private:
    float goalX, goalY;
    bool completed;
};

#endif
