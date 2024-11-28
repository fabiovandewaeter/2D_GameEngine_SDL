#ifndef camera_hpp
#define camera_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

// source : https://lazyfoo.net/tutorials/SDL/30_scrolling/index.php
// Texture wrapper class
class Camera
{
public:
    Camera();
    ~Camera();

    void init(int width, int height, double minScale, double maxScale, int positionX, int positionY);
    void handleEvents(SDL_Event *event);
    void update();
    void move();

    void convertInGameToCameraCoordinates(SDL_Rect &rect);
    void convertCameraToInGameCoordinates(int &x, int &y);
    bool isVisible(SDL_Rect rect);

    void setPosition(int x, int y);
    int getPositionX();
    int getPositionY();
    int getWidth();
    int getHeight();
    double getScale();

    void calculateScales();
    void nextScale();
    void previousScale();

private:
    // Image dimensions
    int width, height;
    int velX, velY;
    double scale, scaleSpeed, minScale, maxScale;

    // in-game coordinates
    int positionX, positionY;
    int velocity;
    int sprintVelocity;

    int scaleIndex;
    std::vector<double> validScales;
};

#endif
