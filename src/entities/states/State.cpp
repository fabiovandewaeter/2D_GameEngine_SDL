#include "entities/states/State.hpp"

State::State() {}
State::State(Entity *entity)
{
    init(entity);
}
State::~State() {}

void State::init(Entity *entity)
{
    this->entity = entity;
}
void State::update()
{
}