#ifndef text_manager_hpp
#define text_manager_hpp

#define NUMBER_OF_CHARACTERS 94

#include <SDL2/SDL_ttf.h>

class Texture;

// generate text
class TextManager
{
public:
    TextManager();
    ~TextManager();

    void init(SDL_Renderer *renderer);
    void loadAllCharactersTextures();

    Texture *generateTextTexture(const char *text);
    void free();

private:
    SDL_Renderer *renderer;
    TTF_Font *font;
    Texture *allCharacterTextures[NUMBER_OF_CHARACTERS];
};

#endif