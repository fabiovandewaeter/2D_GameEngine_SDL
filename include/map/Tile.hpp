#ifndef tile_hpp
#define tile_hpp

#include <SDL2/SDL_rect.h>
#include <vector>

class Texture;
class Camera;

class Tile
{
public:
    Tile(Texture *texture, SDL_Rect box);
    ~Tile();

    void render(Camera *camera);
    int getCenterX();
    int getCenterY();
    int getTextureId();
    SDL_Rect getBox();

private:
    Texture *texture;
    SDL_Rect box;
};

#endif