#ifndef io_manager_hpp
#define io_manager_hpp

#define TOTAL_DATA 1000

#include <SDL2/SDL_stdinc.h>


// source : https://lazyfoo.net/tutorials/SDL/33_file_reading_and_writing/index.php
class IOManager
{
public:
    IOManager();
    ~IOManager();
    
private:
    Sint32 data[TOTAL_DATA];
};

#endif