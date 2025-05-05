#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Bird.h"
#include "Rabbit.h"
#include "Cat.h"
#include "Wall.h"
#include "AudioManager.h"
#include "TextRenderer.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void handleEvents();
    void update();
    void render();
    static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer);

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* background;
    SDL_Texture* wallTexture;
    SDL_Texture* birdTexture;
    SDL_Texture* rabbitTexture;
    SDL_Texture* catTexture;
    SDL_Texture* endTexture;

    Bird* bird;
    Rabbit* rabbit;
    Cat* cat;

    std::vector<Wall> walls;
    AudioManager audioManager;
    TextRenderer* textRenderer;

    int birdScore;
    int rabbitScore;
    int catScore;

    bool running;
    bool gameEnded;

    int winner;
};

#endif
