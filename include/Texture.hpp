#ifndef texture_hpp
#define texture_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

// source : https://lazyfoo.net/tutorials/SDL/10_color_keying/index.php
// Texture wrapper class
class Texture
{
public:
    Texture();
    Texture(SDL_Renderer *renderer);
    ~Texture();

    Texture *loadFromFile(std::string path);
    Texture *loadFromRenderedText(TTF_Font *font, std::string text, SDL_Color textColor);
    void free();
    void render(SDL_Rect renderBox);

    int getWidth();
    int getHeight();
    void setSize(int width, int height);
    int getCenterX();
    int getCenterY();
    int getId();

private:
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    int width, height;
    int id;
};

#endif