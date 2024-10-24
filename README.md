# 2D GameEngine SDL

## Description

A small 2D game engine written in c++ and using SDL

The basic functionalities of a game engine are recreated from scratch in order to obtain maximum performance on specific game functionalities that I want to create

## Commands

### Windows

`g++ src/*.cpp src/entities/*.cpp src/entities/states/*.cpp src/entities/AI/*.cpp src/entities/factions/*.cpp src/systems/*.cpp src/map/*.cpp src/structures/*.cpp src/structures/passiveStructures/*.cpp src/structures/activeStructures/*.cpp src/actions/patterns/*.cpp src/actions/effects/*.cpp src/actions/*.cpp -o Windows_bin/main -I include -L Windows_lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer ; .\Windows_bin\main.exe 60`

### MacOs

#### Install

- `brew install sdl2_image`
- `brew install sdl2_tt`
- `brew install sdl2_mixer`


`g++ src/*.cpp src/entities/*.cpp src/entities/states/*.cpp src/entities/AI/*.cpp src/entities/factions/*.cpp src/systems/*.cpp src/map/*.cpp src/structures/*.cpp src/structures/passiveStructures/*.cpp src/structures/activeStructures/*.cpp src/actions/patterns/*.cpp src/actions/effects/*.cpp src/actions/*.cpp -I include -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_image/include/SDL2/ -I/opt/homebrew/opt/sdl2_ttf/include/SDL2 -I/opt/homebrew/opt/sdl2_mixer/include/SDL2 -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/opt/sdl2_image/lib -L/opt/homebrew/opt/sdl2_ttf/lib -L/opt/homebrew/opt/sdl2_mixer/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer ; ./a.out 500 500`

### Linux (no root)

#### Install

* SDL2
```bash
git clone https://github.com/libsdl-org/SDL.git -b SDL2
./autogen.sh
./configure --prefix=$HOME/libs/SDL2
make
make install
export LD_LIBRARY_PATH=$HOME/libs/SDL2/lib:$LD_LIBRARY_PATH
```
* SDL_image:
```bash
wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.tar.gz
tar -xzf SDL2_image-2.0.5.tar.gz
./configure --prefix=$HOME/libs/SDL2
make
make install
```
* SDL_ttf:
```bash
wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.22.0.tar.gz
tar -xzf SDL2_ttf-2.22.0.tar.gz
./configure --prefix=$HOME/libs/SDL2
make
make install
```
* SDL_mixer:
```bash
wget https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.8.0.tar.gz
tar -xzf SDL2_mixer-2.8.0.tar.gz
./configure --prefix=$HOME/libs/SDL2
make
make install
```

`g++ src/*.cpp src/entities/*.cpp src/entities/states/*.cpp src/entities/AI/*.cpp src/entities/factions/*.cpp src/systems/*.cpp src/map/*.cpp src/structures/*.cpp src/structures/passiveStructures/*.cpp src/structures/activeStructures/*.cpp src/actions/patterns/*.cpp src/actions/effects/*.cpp src/actions/*.cpp -I include -I$HOME/libs/SDL2/include -L$HOME/libs/SDL2/lib -lSDL2 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer`

## SDL libraries
- SDL2
- SDL_image
- SDL_ttf
- SDL_mixer
