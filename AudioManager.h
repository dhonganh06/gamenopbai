#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool loadMusic(const std::string& filename);
    void playMusic();
    void stopMusic();

private:
    Mix_Music* music;
    bool audioInitialized;
};

#endif
