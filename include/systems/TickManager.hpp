#ifndef tick_manager_hpp
#define tick_manager_hpp

#include <SDL2/SDL.h>

class TickManager
{
public:
    ~TickManager();

    static TickManager *getInstance();

    void handleTickSpeed(Uint64 gameFrameDelay);
    Uint64 getNormalizedTick();
    void setFrameStart();
    Uint64 getTicks();

private:
    TickManager();
    static TickManager *instance;

    Uint64 ticks;
    Uint64 frequency;
    Uint64 frameStart;
};

#endif