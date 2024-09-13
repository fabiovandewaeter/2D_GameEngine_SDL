#include "MouseManager.hpp"

MouseManager::MouseManager(){}
MouseManager::~MouseManager() {}

void MouseManager::init(Camera *camera){
	this->camera = camera;
}

// source : https://lazyfoo.net/tutorials/SDL/17_mouse_events/index.php
void MouseManager::handleEvents(SDL_Event *event){
	//If mouse event happened
	if( event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );        //Check if mouse is in button
						    //Set mouse over sprite
		switch( event->type )
		{
			case SDL_MOUSEMOTION:
				break;

			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_MOUSEBUTTONUP:
				break;
		}
	this->camera->convertCameraToInGameCoordinates(x, y);
	std::cout << x << " " << y << std::endl;
	}
}
