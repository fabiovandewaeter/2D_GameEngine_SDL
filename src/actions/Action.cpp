#include "actions/Action.hpp"

#include "actions/patterns/Pattern.hpp"
#include "actions/effects/Effect.hpp"
#include "systems/TickManager.hpp"

Action::Action(Pattern *pattern, std::vector<Effect *> effects, Uint64 cooldown)
{
    this->pattern = pattern;
    this->effects = effects;
    this->cooldown = cooldown;
    this->cooldownStartTick = 0;
    this->tickManager = TickManager::getInstance();
}
Action::~Action()
{
    delete pattern;
    int size = this->effects.size();
    for (int i = 0; i < size; i++)
    {
        delete this->effects[i];
    }
}

void Action::apply(int x, int y)
{
    if ((this->tickManager->getTicks() - this->cooldownStartTick) > this->cooldown)
    {
        std::vector<Entity *> entities = this->pattern->getAffectedEntities(x, y);
        int size = entities.size();
        int sizeEffectsList = this->effects.size();
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < sizeEffectsList; j++)
            {
                this->effects[j]->applyEffect(entities[i]);
            }
        }
        this->cooldownStartTick = this->tickManager->getTicks();
    }
}