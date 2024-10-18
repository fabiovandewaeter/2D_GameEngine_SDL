#ifndef action_hpp
#define action_hpp

#include <vector>
#include <SDL2/SDL_stdinc.h>

class Entity;
class Pattern;
class Effect;
class TickManager;

class Action
{
public:
    Action(Pattern *pattern, std::vector<Effect *> effects, Uint64 cooldown);
    ~Action();

    void apply(int x, int y);

private:
    Pattern *pattern;
    std::vector<Effect *> effects;
    int cooldown;
    int cooldownStartTick;
    TickManager *tickManager;
};

#endif