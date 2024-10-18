#include "systems/AudioManager.hpp"

#include <iostream>

AudioManager::AudioManager() {}
AudioManager::~AudioManager() {}
void AudioManager::free()
{
    int size = this->music.size();
    for (int i = 0; i < size; i++)
    {
        Mix_FreeMusic(this->music[i]);
        this->music[i] = NULL;
    }
}

void AudioManager::init()
{
}
void AudioManager::loadMedia()
{
    this->music.push_back(Mix_LoadMUS("assets/audio/music/Evangelion.wav"));
    Mix_PlayMusic(this->music[0], -1);
}

std::vector<Mix_Music *> *AudioManager::getMusic()
{
    return &this->music;
}