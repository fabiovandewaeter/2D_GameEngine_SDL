#include "Game.hpp"

#include "structures/activeStructures/Core.hpp"
#include "structures/activeStructures/Turret.hpp"
#include "map/Map.hpp"
#include "map/Chunk.hpp"
#include "entities/Player.hpp"
#include "Texture.hpp"

SDL_Event event;

Game::Game()
{
}
Game::~Game()
{
}

void Game::init(std::string title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    this->fixedFPS = 60;
    this->fixedUPS = 60;
    this->screenWidth = width;
    this->screenHeight = height;
    // initialize window
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    this->isRunning = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems Initialised" << std::endl;
        // Create window
        if (!(this->window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags)))
        {
            std::cout << "FAIL : Window NOT created" << std::endl;
            isRunning = false;
        }
        // Create renderer
        if ((this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED)))
        {
            SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
            std::cout << "Renderer created" << std::endl;
        }
        else
        {
            std::cout << "FAIL : Renderer NOT created" << std::endl;
            isRunning = false;
        }
        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            std::cout << "FAIL : SDL_image NOT initialized" << std::endl;
            isRunning = false;
        }
        // Initialize SDL_ttf
        if (TTF_Init() == -1)
        {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            isRunning = false;
        }
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            isRunning = false;
        }
    }
    // window icon
    SDL_Surface *iconSurface = SDL_LoadBMP("assets/icon/window_icon.bmp");
    if (!iconSurface)
    {
        std::cout << "Failed to load icon: " << IMG_GetError() << std::endl;
    }
    SDL_SetWindowIcon(this->window, iconSurface);
    SDL_FreeSurface(iconSurface);
    this->camera.init(width, height, 10, 200000000, 0, 0);
    this->collisionManager.init(&this->map, &this->entityManager);
    loadMedia();
    this->entityManager.init(&this->camera, &this->collisionManager, this->entityTextures);
    this->map.init(&this->camera, this->tileTextures, this->passiveStructureTextures, this->activeStructureTextures, &this->perlinNoise, &this->collisionManager);

    // TO DELETE
    this->core = new Core((*this->activeStructureTextures)[0], &this->collisionManager, &this->entityManager, (SDL_Rect){16 * 2, 16 * 2, 0, 0}, 1000, nullptr);
    this->turret = new Turret((*this->activeStructureTextures)[1], &this->collisionManager, &this->entityManager, (SDL_Rect){16 * 3, 16 * 3, 0, 0}, 1000, nullptr);
    this->map.getChunk(0, 0)->addStructure(this->core);
    this->map.getChunk(0, 0)->addStructure(this->turret);
    // TO DELETE

    this->mouseManager.init(&this->camera, &this->map);
    this->textManager.init(this->renderer);
    loadEntities();
}

void Game::loadMedia()
{
    // textures
    this->textureManager.init(this->renderer);
    this->textureManager.loadMedia();
    this->backgroundTexture = this->textureManager.getBackgroundTexture();
    this->entityTextures = this->textureManager.getEntityTextures();
    this->tileTextures = this->textureManager.getTileTextures();
    this->passiveStructureTextures = this->textureManager.getPassiveStructureTextures();
    this->activeStructureTextures = this->textureManager.getActiveStructureTextures();

    // audio
    this->audioManager.init();
    this->audioManager.loadMedia();
    this->music = this->audioManager.getMusic();
}
void Game::loadEntities()
{
    this->entityManager.loadEntities();
    this->player = new Player((*this->entityTextures)[0], (SDL_Rect){0, 0, 16, 16}, 100);
    this->entityManager.addEntity(this->player);
}

void Game::handleEvents()
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            this->isRunning = false;
        }
        this->camera.handleEvents(&event);
        this->player->handleEvents(&event);
        this->mouseManager.handleEvents(&event);
    }
}

Uint64 lastTimeUPS = SDL_GetTicks64(), counterUPS = 0, intervalUPS = 1000;
Uint64 lastTimeUPSLimiter = SDL_GetTicks64(), counterUPSLimiter = 0;
void Game::update()
{
    // if (limiter("UPS", counterUPSLimiter, 1000 / this->fixedUPS, lastTimeUPSLimiter))
    this->player->update(&this->collisionManager);
    this->camera.update();
    this->entityManager.update();

    this->map.update();

    //this->core->update();
    //this->turret->update();

    countPrinter("UPS", counterUPS, intervalUPS, lastTimeUPS);
}

Uint64 lastTimeFPS = SDL_GetTicks64(), counterFPS = 0, intervalFPS = 1000;
Uint64 lastTimeFPSLimiter = SDL_GetTicks64(), counterFPSLimiter = 0;
void Game::render()
{
    if (limiter("FPS", counterFPSLimiter, 1000 / this->fixedFPS, lastTimeFPSLimiter))
    {
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        SDL_RenderClear(this->renderer);

        double scale = this->camera.getScale();
        this->backgroundTexture->render((SDL_Rect){(int)((this->screenWidth / 2) - (this->backgroundTexture->getCenterX() * scale)), (int)((this->screenHeight / 2) - (this->backgroundTexture->getCenterY() * scale)), (int)(this->backgroundTexture->getWidth() * scale), (int)(this->backgroundTexture->getHeight() * scale)});
        // tiles and static objects
        this->map.render();

        // entities
        this->entityManager.render();
        this->player->render(&this->camera);

        SDL_RenderPresent(this->renderer);
        countPrinter("FPS", counterFPS, intervalFPS, lastTimeFPS);
    }
}

void Game::clean()
{
    this->textureManager.free();
    this->map.free();
    this->audioManager.free();

    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::running()
{
    return this->isRunning;
}
void Game::setFPS(unsigned int fps)
{
    this->fixedFPS = fps;
}
void Game::setUPS(unsigned int ups)
{
    this->fixedUPS = ups;
    this->frameDelay = 10000000 / this->fixedUPS;
}
Uint64 Game::getFrameDelay()
{
    return this->frameDelay;
}
bool Game::limiter(std::string name, Uint64 &counter, Uint64 interval, Uint64 &lastTime)
{
    bool res = false;
    counter++;
    Uint64 currentTime = SDL_GetTicks64();
    Uint64 deltaTime = currentTime - lastTime;
    if (deltaTime >= interval)
    {
        lastTime = currentTime;
        counter = 0;
        res = true;
    }
    return res;
}
void Game::countPrinter(std::string name, Uint64 &counter, Uint64 interval, Uint64 &lastTime)
{
    counter++;
    Uint64 currentTime = SDL_GetTicks64();
    Uint64 deltaTime = currentTime - lastTime;
    if (deltaTime >= interval)
    { // 1000 ms = 1 seconde
        std::cout << name << ": " << counter / (deltaTime / 1000.0f) << std::endl;
        lastTime = currentTime;
        counter = 0;
    }
}