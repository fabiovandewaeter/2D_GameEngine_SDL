#ifndef chunk_hpp
#define chunk_hpp

#define SIZE 16

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

class Map;
class Texture;
class PerlinNoise;
class CollisionManager;
class Camera;
class Tile;
class Structure;
class Faction;

class Chunk
{
public:
    Chunk(int positionX, int positionY, int tileSize, Map *map, std::vector<Texture *> *tileTextures, std::vector<Texture *> *passiveStructureTextures, std::vector<Texture *> *activeStructureTextures, PerlinNoise *perlinNoise, CollisionManager *collisionManager);
    ~Chunk();

    void loadTiles();
    void loadTilesDefault();
    void loadTilesWithPerlinNoise();
    void loadPassiveStructures();
    void loadActiveStructures();
    void generateCombinedTexture(SDL_Renderer *renderer);
    void render(Camera *camera);
    void update();

    Tile *getTile(int x, int y);
    Structure *getStructure(int x, int y);
    bool isStructure(int x, int y);
    void addStructure(Structure *structure);
    void addWall(int x, int y);
    void destroyStructure(int x, int y);
    void setFaction(Faction *faction);

private:
    Tile *allTiles[SIZE * SIZE];
    std::vector<Texture *> *tileTextures;
    SDL_Texture *combinedTexture = nullptr;
    int tileSize;

    Map *map;
    std::unordered_map<std::string, Structure *> allStructures;
    std::vector<Texture *> *passiveStructureTextures, *activeStructureTextures;

    int positionX, positionY;
    SDL_Rect box;
    PerlinNoise *perlinNoise;

    Faction *faction;

    void convertToTileCoordinates(int &x, int &y);






    SDL_Renderer *renderer;
};

#endif