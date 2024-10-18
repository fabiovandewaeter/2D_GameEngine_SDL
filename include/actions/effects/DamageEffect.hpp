#ifndef damage_effect_hpp
#define damage_effect_hpp

#include "Effect.hpp"

class DamageEffect : public Effect
{
public:
    DamageEffect(int damage);
    ~DamageEffect();

    void applyEffect(Entity *entity) override;

private:
    int damage;
};

#endif