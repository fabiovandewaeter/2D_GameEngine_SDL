#include "systems/TextManager.hpp"

#include "Texture.hpp"

TextManager::TextManager() {}
TextManager::~TextManager()
{
    free();
}

void TextManager::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    this->font = TTF_OpenFont("assets/fonts/Raleway-Medium.ttf", 28);
    loadAllCharactersTextures();
}

void TextManager::loadAllCharactersTextures()
{
    for (char i = 32; i < 127; i++)
    {
        char character[2] = {i, 0};
        this->allCharacterTextures[i - 32] = generateTextTexture(character);
    }
}

Texture *TextManager::generateTextTexture(const char *text)
{
    return (new Texture(this->renderer))->loadFromRenderedText(this->font, text, (SDL_Color){0, 0, 0});
}

void TextManager::free()
{
    TTF_CloseFont(this->font);
}