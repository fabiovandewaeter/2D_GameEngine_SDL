#ifndef heal_effect_hpp
#define heal_effect_hpp

#include "Effect.hpp"

class HealEffect : public Effect
{
public:
    HealEffect(int hp) : hp(hp) {}
    ~HealEffect() = default;

    void applyEffect(Entity *entity) override;

private:
    int hp;
};

#endif
