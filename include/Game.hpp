#ifndef game_hpp
#define game_hpp


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <time.h>
#include "map/Map.hpp"
#include "systems/Camera.hpp"
#include "systems/TextManager.hpp"
#include "systems/TextureManager.hpp"
#include "systems/CollisionManager.hpp"
#include "systems/IOManager.hpp"
#include "systems/PerlinNoise.hpp"
#include "systems/MouseManager.hpp"
#include "systems/EntityManager.hpp"
#include "systems/AudioManager.hpp"
#include "systems/ItemManager.hpp"

class Texture;
class Core;
class Turret;
class Player;

class Game
{
public:
    Game();
    ~Game();

    void init(std::string title, const int& xpos, const int& ypos, const int& width, const int& height, const bool& fullscreen, const bool& vsync);
    void loadMedia();
    void loadEntities();
    void loadItems();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();
    bool limiter(std::string name, const Uint64 &counter, const Uint64 interval, const Uint64 &lastTime);
    void countPrinter(std::string name, const Uint64 &counter, const Uint64 interval, const Uint64 &lastTime);
    void setFPS(const unsigned int& fps);
    void setUPS(const unsigned int& ups);
    Uint64 getFrameDelay();

private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Camera camera;
    int screenWidth, screenHeight;
    unsigned int fixedFPS;
    unsigned int fixedUPS;
    Uint64 frameDelay;

    Player *player;
    Texture *backgroundTexture;
    std::vector<Texture *> *entityTextures;
    std::vector<Texture *> *tileTextures;
    std::vector<Texture *> *passiveStructureTextures;
    std::vector<Texture *> *activeStructureTextures;

    std::vector<Mix_Music *> *music;

    Core *core;
    Turret *turret;

    TextManager textManager;
    TextureManager textureManager;
    CollisionManager collisionManager;
    IOManager ioManager;
    Map map;
    PerlinNoise perlinNoise;
    MouseManager mouseManager;
    EntityManager entityManager;
    AudioManager audioManager;
    ItemManager itemManager;

    SDL_Surface *loadSurface(std::string path);
    SDL_Texture *loadTexture(std::string path);
};

#endif
