#include "AudioManager.h"

AudioManager::AudioManager() : music(nullptr), audioInitialized(false) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    int flags = MIX_INIT_OGG;
    if ((Mix_Init(flags) & flags) != flags) {
        std::cerr << "SDL_mixer could not initialize OGG support! Mix_Init Error: " << Mix_GetError() << std::endl;
        return;
    }

    audioInitialized = true;
}

AudioManager::~AudioManager() {
    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }

    if (audioInitialized) {
        Mix_CloseAudio();
        Mix_Quit();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

bool AudioManager::loadMusic(const std::string& filename) {
    if (!audioInitialized) return false;

    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }

    music = Mix_LoadMUS(filename.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << filename << " | Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void AudioManager::playMusic() {
    if (!audioInitialized || !music) return;

    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
    }
}

void AudioManager::stopMusic() {
    if (audioInitialized && Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}

