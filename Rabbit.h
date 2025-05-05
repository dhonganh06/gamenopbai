#ifndef RABBIT_H
#define RABBIT_H

#include <SDL.h>

class Rabbit {
public:
    Rabbit(SDL_Texture* texture, int x, int y, int screenWidth, int screenHeight);
    void update();
    void render(SDL_Renderer* renderer);
    SDL_Rect getBodyRect(); // dùng cho kiểm tra va chạm

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;

    bool facingRight;

    int frame;
    int frameWidth, frameHeight;
    int totalFrames;
    int screenWidth;
    int screenHeight;

    float posX;
    float posY;
    float moveSpeed;

    Uint32 lastFrameTime;
    const int animationSpeed = 150;

    Uint32 moveTimer;
    Uint32 moveInterval;
    int moveDirX;
    int moveDirY;
};

#endif
