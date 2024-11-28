#include "Texture.hpp"

int counter = 0;
Texture::Texture() {}
Texture::Texture(SDL_Renderer *renderer)
{
    // Initialize
    this->texture = NULL;
    this->width = 0;
    this->height = 0;
    this->renderer = renderer;
    this->id = counter;
    counter++;
}
Texture::~Texture()
{
    free();
}
void Texture::free()
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

Texture *Texture::loadFromFile(std::string path)
{
    // Get rid of preexisting texture
    free();
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(this->renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            this->width = loadedSurface->w;
            this->height = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return this;
}
Texture *Texture::loadFromRenderedText(TTF_Font *font, std::string text, SDL_Color textColor)
{
    free();
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        // Create texture from surface pixels
        this->texture = SDL_CreateTextureFromSurface(this->renderer, textSurface);
        if (this->texture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            // Get image dimensions
            this->width = textSurface->w;
            this->height = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    return this;
}

void Texture::render(int x, int y)
{
    SDL_Rect renderBox = {x, y, this->width, this->height};
    render(renderBox);
}
void Texture::render(int x, int y, int w, int h)
{
    SDL_Rect renderBox = {x, y, w, h};
    render(renderBox);
}
void Texture::render(SDL_Rect renderBox)
{
    // 1px padding
    renderBox.x -= 1;
    renderBox.y -= 1;
    renderBox.w += 2;
    renderBox.h += 2;

    SDL_RenderCopy(this->renderer, texture, NULL, &renderBox);
}
void Texture::render(SDL_Rect srcBox, SDL_Rect dstBox)
{
    SDL_RenderCopy(this->renderer, texture, &srcBox, &dstBox);
}
void Texture::render(SDL_Renderer *renderer, SDL_Rect srcBox, SDL_Rect dstBox)
{
        if (this == nullptr)
{
    std::cerr << "Erreur : this est nul !" << std::endl;
    return;
}
        if (this->texture == nullptr)
    {
        std::cerr << "Erreur : texture est nulle !" << std::endl;
        return;
    }
        if (renderer == nullptr)
    {
        std::cerr << "Erreur : texture est nulle !" << std::endl;
        return;
    }

    SDL_RenderCopy(this->renderer, texture, NULL, &dstBox);
}


int Texture::getWidth() { return width; }
int Texture::getHeight() { return height; }
void Texture::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}
int Texture::getCenterX() { return this->getWidth() / 2; }
int Texture::getCenterY() { return this->getHeight() / 2; }
int Texture::getId() { return this->id; }
int Texture::getTextureDefaultSize() { return TEXTURE_DEFAULT_SIZE; }
SDL_Renderer * Texture::getRenderer(){
    std::cout << "REMOVE Texture::getRenderer()" << std::endl;
    return this->renderer;
}