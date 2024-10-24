#ifndef active_structure_hpp
#define active_structure_hpp

#include "../Structure.hpp"
#include <vector>

class EntityManager;
class TextureManager;
class Map;
class Action;
class Faction;

class ActiveStructure : public Structure
{
public:
    ActiveStructure(Texture *texture, CollisionManager *collisionManager, EntityManager *entityManager, SDL_Rect hitBox, unsigned int HP, bool solid, Faction *faction);
    ~ActiveStructure();

    virtual void update() = 0;
    void onLeftClick() override;

protected:
    CollisionManager *collisionManager;
    EntityManager *entityManager;
    bool active;
    std::vector<Action *> actions;
    Faction *faction;
};

#endif