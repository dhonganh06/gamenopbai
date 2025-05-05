#ifndef BIRD_H
#define BIRD_H

#include <SDL.h>
#include <vector>
#include "Wall.h"

class Bird {
public:
    Bird(SDL_Texture* texture, int x, int y, int screenWidth, int screenHeight);
    void handleEvent(const SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);
    void checkWallCollisions(std::vector<Wall>& walls, int& birdScore);

    SDL_Rect getBodyRect();

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    SDL_Rect beakRect;
    bool facingRight;

    int frame;
    int frameWidth, frameHeight;
    int totalFrames;
    int screenWidth;
    int screenHeight;

    float posX;
    float posY;
    float velocityX;
    float velocityY;
    float gravity;
    float flapStrength;
    float moveSpeed;

    Uint32 lastFrameTime;
    const int animationSpeed = 100;
};

#endif
