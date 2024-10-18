#ifndef controlled_state_hpp
#define controlled_state_hpp

#include "State.hpp"

class ControlledState : public State
{
public:
    ControlledState(Entity *entity);
    ~ControlledState();

private:
};

#endif