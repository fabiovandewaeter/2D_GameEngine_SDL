#include "Camera.hpp"

const int VELOCITY = 1;
const int SPRINT_VELOCITY = 2;
// 1 if false and SPRINT_VELOCITY if true
int sprint = 1;
int leftVelX = 0, rightVelX = 0, upVelY = 0, downVelY = 0;

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
            upVelY = VELOCITY;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            downVelY = VELOCITY;
            break;
        case SDLK_LEFT:
        case SDLK_q:
            leftVelX = VELOCITY;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            rightVelX = VELOCITY;
            break;
        case SDLK_LSHIFT:
            sprint = SPRINT_VELOCITY;
            break;
        }
        this->velX = sprint*(rightVelX - leftVelX);
        this->velY = sprint*(downVelY - upVelY);
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
            sprint = 1;
            break;
        }
        this->velX = sprint*(rightVelX - leftVelX);
        this->velY = sprint*(downVelY - upVelY);
    }
}