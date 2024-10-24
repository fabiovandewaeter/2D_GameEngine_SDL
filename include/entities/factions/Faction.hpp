#ifndef faction_hpp
#define faction_hpp

#include <string>
#include <vector>

class Entity;

class Faction
{
public:
    Faction(std::string name);
    ~Faction();

private:
    std::string name;
    std::vector<Entity *> members;
};

#endif