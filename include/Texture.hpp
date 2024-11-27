#ifndef texture_hpp
#define texture_hpp

#define TEXTURE_DEFAULT_SIZE 32

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
    void render(int x, int y);
    void render(int x, int y, int w, int h);
    void render(SDL_Rect renderBox);
    void render(SDL_Rect srcBox, SDL_Rect dstBox);
    void render(SDL_Renderer *renderer, SDL_Rect srcBox, SDL_Rect dstBox);

    int getWidth();
    int getHeight();
    void setSize(int width, int height);
    int getCenterX();
    int getCenterY();
    int getId();
    int getTextureDefaultSize();


    SDL_Renderer * getRenderer();

private:
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    int width, height;
    int id;
};

#endif