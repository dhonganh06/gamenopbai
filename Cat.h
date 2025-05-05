#ifndef CAT_H
#define CAT_H

#include <SDL.h>
#include <vector>
#include "Wall.h"

class Cat {
public:
    Cat(SDL_Texture* texture, int x, int y, int screenWidth, int screenHeight);
    void handleEvent(const SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);
    SDL_Rect getBodyRect();
    void checkWallCollisions(std::vector<Wall>& walls, int& catScore);

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;

    bool facingRight;
    bool jumping;

    int frame;
    int frameWidth, frameHeight;
    int totalFrames;
    int screenWidth;
    int screenHeight;

    float posX;
    float posY;
    float velocityX;
    float velocityY;
    float moveSpeed;
    float jumpStrength;
    float gravity;

    Uint32 lastFrameTime;
    const int animationSpeed = 150;
};

#endif

