#include "Game.hpp"

#include "systems/TickManager.hpp"

Game game;

int main(int argc, char *argv[])
{

    int width = 800;
    int height = 600;
    unsigned int UPS = 60;
    std::cout << argc << " " << argv[1] << std::endl;
    if (argc == 2)
    {
        UPS = std::atoi(argv[1]);
    }
    else if (argc == 3)
    {
        width = std::atoi(argv[1]);
        height = std::atoi(argv[2]);
    }
    else if (argc == 4)
    {
        width = std::atoi(argv[1]);
        height = std::atoi(argv[2]);
        UPS = std::atoi(argv[3]);
    }
    std::cout << "Window width: " << width << std::endl;
    std::cout << "Window height: " << height << std::endl;

    game.init("TestEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false);
    // game.init("TestEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, false);
    game.setUPS(UPS);

    TickManager *tickManager = TickManager::getInstance();

    while (game.running())
    {
        tickManager->setFrameStart();
        game.handleEvents();
        game.update();
        game.render();

        tickManager->handleTickSpeed(game.getFrameDelay());
    }

    game.clean();
    return 0;
}
