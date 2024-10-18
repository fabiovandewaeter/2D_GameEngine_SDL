#ifndef rest_state_hpp
#define rest_state_hpp

#include "State.hpp"

class RestState : public State
{
public:
    RestState();
    RestState(Entity *entity);
    ~RestState();
private:
};

#endif