#include "systems/EntityManager.hpp"

#include "systems/Camera.hpp"
#include "systems/CollisionManager.hpp"
#include "Texture.hpp"
#include "entities/Entity.hpp"
#include "entities/Entity.hpp"

EntityManager::EntityManager() {}
EntityManager::~EntityManager() {}

void EntityManager::init(Camera *camera, CollisionManager *collisionManager, std::vector<Texture *> *entityTextures)
{
    this->collisionManager = collisionManager;
    this->entityTextures = entityTextures;
    this->camera = camera;
}

void EntityManager::loadEntities()
{
    //this->entities.push_back(new Entity((*this->entityTextures)[1], (SDL_Rect){50, 50, 16, 16}, 100));
}
void EntityManager::update()
{
    int size = this->entities.size();
    for (int i = 0; i < size; i++)
    {
        this->entities[i]->update(this->collisionManager);
    }
}
void EntityManager::render()
{
    int size = this->entities.size();
    for (int i = 0; i < size; i++)
    {
        entities[i]->render(this->camera);
    }
}

void EntityManager::addEntity(Entity *entity)
{
    this->entities.push_back(entity);
    std::cout << this->entities.size() << std::endl;
}
bool EntityManager::checkCollision(SDL_Rect rectA, SDL_Rect rectB)
{
    return !(rectA.x + rectA.w <= rectB.x ||
             rectA.x >= rectB.x + rectB.w ||
             rectA.y + rectA.h <= rectB.y ||
             rectA.y >= rectB.y + rectB.h);
}
std::vector<Entity *> EntityManager::getPotentialEntities(Entity *entity)
{
    return this->entities;
}
std::vector<Entity *> EntityManager::getEntitiesInArea(SDL_Rect area)
{
    std::vector<Entity *> res;
    int size = this->entities.size();
    for (int i = 0; i < size; i++)
    {
        SDL_Rect hitBox = this->entities[i]->getHitBox();
    //std::cout << area.x << " " << area.y << " " << area.w << " " << area.h << " " << hitBox.x << " " << hitBox.y << " " << hitBox.w << " " << hitBox.h << std::endl;
        if (checkCollision(this->entities[i]->getHitBox(), area))
        {
            res.push_back(this->entities[i]);
        }
    }
    return res;
}
Entity *EntityManager::generateDefaultEntity(SDL_Rect hitBox)
{
    return new Entity((*this->entityTextures)[0], hitBox, 100);
}