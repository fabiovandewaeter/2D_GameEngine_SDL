#include "Game.hpp"

int main(int argc, char *argv[])
{
    unsigned int width = 800;
    unsigned int height = 600;
    unsigned int UPS = 60;
    int vsync = 1;
    if (argc == 2)
    {
        UPS = std::atoi(argv[1]);
    }
    if (argc >= 3)
    {
        width = std::atoi(argv[1]);
        height = std::atoi(argv[2]);
    }
    if (argc >= 4)
    {
        UPS = std::atoi(argv[3]);
    }
    if (argc >= 5)
    {
        vsync = std::atoi(argv[4]);
    }

    Game *game = new Game("TestEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false, vsync, UPS);
    game->run(); // Game loop
    game->clean();
    return 0;
}
