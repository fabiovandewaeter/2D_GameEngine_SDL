#include "entities/actions/KillEntityAction.hpp"

#include "systems/algorithms/AstarPathFinding.hpp"
#include "map/Map.hpp"
#include "entities/actions/MoveAction.hpp"
#include "entities/actions/AttackEntityAction.hpp"

KillEntityAction::KillEntityAction(Entity *attacker, Entity *target) : Action(attacker), target(target), state(State::INIT)
{
    std::cout << "NEW" << std::endl;
    if (!target)
    {
        std::cerr << "ERROR : KillEntityAction => no Entity found on the Map" << std::endl;
        state = State::FINISHED;
    }
}

void KillEntityAction::update()
{
    if (!target || target->isDead())
    {
        std::cout << "KillEntityAction::execute() : Target is dead" << std::endl;
        state = State::FINISHED;
        return;
    }

    switch (state)
    {
    case State::INIT:
        moveAction = std::make_unique<MoveAction>(target->getPositionX(), target->getPositionY(), actor);
        state = State::MOVING;
        break;
    case State::MOVING:
        if (moveAction)
        {
            moveAction->update();
            float distance = calculateDistance(actor->getPositionX(), actor->getPositionY(), target->getPositionX(), target->getPositionY());
            if (distance <= actor->getRange() + 1)
            {
                attackEntityAction = std::make_unique<AttackEntityAction>(actor, target);
                state = State::ATTACKING;
            }
            else if (moveAction->isCompleted())
            {
                moveAction = std::make_unique<MoveAction>(target->getPositionX(), target->getPositionY(), actor);
                break;
            }
        }
        break;
    case State::ATTACKING:
        if (attackEntityAction)
        {
            float distance = calculateDistance(actor->getPositionX(), actor->getPositionY(), target->getPositionX(), target->getPositionY());
            if (!(distance <= actor->getRange() + 1))
            {
                moveAction = std::make_unique<MoveAction>(target->getPositionX(), target->getPositionY(), actor);
                state = State::MOVING;
                break;
            }
            attackEntityAction->update();
            if (target->isDead())
            {
                state = State::FINISHED;
            }
        }
        break;
    case State::FINISHED:
    default:
        break;
    }
}

bool KillEntityAction::isCompleted() const { return state == State::FINISHED; }

float KillEntityAction::calculateDistance(float sourceX, float sourceY, float goalX, float goalY)
{
    float dx = goalX - sourceX;
    float dy = goalY - sourceY;
    return std::sqrt(dx * dx + dy * dy);
}
