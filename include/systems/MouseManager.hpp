#ifndef mouse_manager_hpp
#define mouse_manager_hpp

#include <SDL2/SDL.h>

class Camera;
class Map;

class MouseManager
{
public:
	MouseManager();
	~MouseManager();

	void init(Camera *camera, Map *map);
	void handleEvents(SDL_Event *event);

private:
	Camera *camera;
	Map *map;
};

#endif
