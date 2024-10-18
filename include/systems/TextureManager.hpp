#ifndef texture_manager_hpp
#define texture_manager_hpp

#include <SDL2/SDL.h>
#include <vector>

class Texture;

class TextureManager
{
public:
    TextureManager();
    TextureManager(SDL_Renderer *renderer);
    ~TextureManager();
    void free();

    void init(SDL_Renderer *renderer);
    void loadMedia();

    Texture *getBackgroundTexture();
    std::vector<Texture *> *getEntityTextures();
    std::vector<Texture *> *getTileTextures();
    std::vector<Texture *> *getPassiveStructureTextures();
    std::vector<Texture *> *getActiveStructureTextures();

private:
    SDL_Renderer *renderer;
    Texture *backgroundTexture;
    std::vector<Texture *> entityTextures;
    std::vector<Texture *> tileTextures;
    std::vector<Texture *> passiveStructureTextures;
    std::vector<Texture *> activeStructureTextures;

    void loadEntityTextures();
    void loadTileTextures();
    void loadPassiveStructureTextures();
    void loadActiveStructureTextures();
};

#endif