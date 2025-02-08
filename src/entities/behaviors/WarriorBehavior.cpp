#include "entities/behaviors/WarriorBehavior.hpp"

#include "entities/actions/GetResourceAction.hpp"

#include <iostream>
void WarriorBehavior::execute(Entity *entity)
{
    GetResourceAction *getResourceAction = new GetResourceAction("Wood");
    getResourceAction->execute(entity);
}
