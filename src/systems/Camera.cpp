#include "systems/Camera.hpp"

// 1 if false and sprintVelocity if true
float sprint = 0.5;
int leftVelX = 0, rightVelX = 0, upVelY = 0, downVelY = 0;

Camera::Camera() {}
Camera::~Camera() {}

void Camera::init(int width, int height, double minScale, double maxScale, int positionX, int positionY)
{
    this->width = width;
    this->height = height;
    this->scale = 1.0;
    this->minScale = minScale;
    this->maxScale = 1 / maxScale;
    this->scaleSpeed = 1;
    this->positionX = positionX;
    this->positionY = positionY;
    this->velocity = 2;
    this->sprintVelocity = 50;

    this->scaleIndex = 0;
    calculateScales();
}

#include <cmath>
bool hasFractionalPart(double number)
{
    double intPart;
    double fracPart = std::modf(number, &intPart);
    return fracPart != 0.0;
}

void Camera::calculateScales()
{
    /*double step = 0.01;
    for (int i = 1; i < 100; i++)
    {
        double tempo = 16 / (step * i);
        if (!hasFractionalPart(tempo))
        {
            std::cout << tempo << std::endl;
            this->validScales.push_back(step*i);
        }
    }*/
    //this->validScales.push_back(0.5);
    for (int i = 1; i < 100; i++)
    {
        //this->validScales.push_back(34/i);
        this->validScales.push_back(i);
    }
}

void Camera::nextScale()
{
    if (this->scaleIndex < this->validScales.size() - 1)
    {
        this->scaleIndex++;
        this->scale = this->validScales[this->scaleIndex];
    }
}
void Camera::previousScale()
{
    if (this->scaleIndex > 0)
    {
        this->scaleIndex--;
        this->scale = this->validScales[this->scaleIndex];
    }
}

void Camera::handleEvents(SDL_Event *event)
{
    // If a key was pressed
    if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
    {
        // Adjust the velocity
        switch (event->key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_z:
            upVelY = this->velocity;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            downVelY = this->velocity;
            break;
        case SDLK_LEFT:
        case SDLK_q:
            leftVelX = this->velocity;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            rightVelX = this->velocity;
            break;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            sprint = sprintVelocity;
            break;
        case SDLK_0:
            this->scaleIndex = 0;
            this->scale = this->validScales[0];
            break;
        case SDLK_DELETE:
            this->positionX = 0;
            this->positionY = 0;
            break;
        }
        this->velX = sprint * (rightVelX - leftVelX);
        this->velY = sprint * (downVelY - upVelY);
    }
    // if mouse wheel moved
    if (event->type == SDL_MOUSEWHEEL)
    {
        if (event->wheel.y > 0)
        {
            nextScale();
        }
        else if (event->wheel.y < 0)
        {
            previousScale();
        }
    }
    // If a key was released
    else if (event->type == SDL_KEYUP && event->key.repeat == 0)
    {
        // Adjust the velocity
        switch (event->key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_z:
            upVelY = 0;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            downVelY = 0;
            break;
        case SDLK_LEFT:
        case SDLK_q:
            leftVelX = 0;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            rightVelX = 0;
            break;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            sprint = 1;
            break;
        }
        this->velX = sprint * (rightVelX - leftVelX);
        this->velY = sprint * (downVelY - upVelY);
    }
}

void Camera::update()
{
    move();
}

void Camera::move()
{
    this->positionX += this->velocity * velX;
    this->positionY += this->velocity * velY;
}

void Camera::convertInGameToCameraCoordinates(SDL_Rect &rect)
{
    int cameraPositionX = this->positionX;
    int cameraPositionY = this->positionY;
    int viewCenterX = this->width / 2;
    int viewCenterY = this->height / 2;

    int viewPositionX = (viewCenterX - cameraPositionX * scale) + (rect.x * scale);
    int viewPositionY = (viewCenterY - cameraPositionY * scale) + (rect.y * scale);

    rect.x = viewPositionX;
    rect.y = viewPositionY;
    rect.w *= scale;
    rect.h *= scale;
}
void Camera::convertCameraToInGameCoordinates(int &x, int &y)
{
    int cameraPositionX = this->positionX;
    int cameraPositionY = this->positionY;
    int viewCenterX = this->width / 2;
    int viewCenterY = this->height / 2;

    x = (-viewCenterX + cameraPositionX * scale + x) / this->scale;
    y = (-viewCenterY + cameraPositionY * scale + y) / this->scale;
}

bool Camera::isVisible(SDL_Rect rect)
{
    int viewBottomRightPositionX = rect.x + rect.w;
    int viewBottomRightPositionY = rect.y + rect.h;
    if (viewBottomRightPositionX < 0 || viewBottomRightPositionY < 0 || rect.x > this->width || rect.y > this->height)
    {
        return false;
    }
    return true;
}

void Camera::setPosition(int x, int y)
{
    this->positionX = x;
    this->positionY = y;
}
int Camera::getPositionX()
{
    return this->positionX;
}
int Camera::getPositionY()
{
    return this->positionY;
}
int Camera::getWidth()
{
    return this->width;
}
int Camera::getHeight()
{
    return this->height;
}
double Camera::getScale()
{
    return this->scale;
}
