#include "Rabbit.h"
#include <cstdlib>

Rabbit::Rabbit(SDL_Texture* tex, int x, int y, int screenWidth, int screenHeight)
    : texture(tex),
      frame(0),
      totalFrames(3),
      posX(x),
      posY(y),
      moveSpeed(2.0f),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      facingRight(true),
      lastFrameTime(SDL_GetTicks()),
      moveTimer(SDL_GetTicks()),
      moveInterval(1000),
      moveDirX(0),
      moveDirY(0)
{
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    frameWidth = texW / totalFrames;
    frameHeight = texH;

    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {x, y, frameWidth, frameHeight};
}

SDL_Rect Rabbit::getBodyRect() {
    return destRect;
}

void Rabbit::update() {
    Uint32 currentTicks = SDL_GetTicks();

    if (currentTicks - moveTimer >= moveInterval) {
        moveDirX = (std::rand() % 3) - 1;
        moveDirY = (std::rand() % 3) - 1;
        moveTimer = currentTicks;
    }

    posX += moveDirX * moveSpeed;
    posY += moveDirY * moveSpeed;

    if (posX < 0) posX = 0;
    if (posX + destRect.w > screenWidth) posX = screenWidth - destRect.w;
    if (posY < 0) posY = 0;
    if (posY + destRect.h > screenHeight) posY = screenHeight - destRect.h;

    if (currentTicks - lastFrameTime >= animationSpeed) {
        frame = (frame + 1) % totalFrames;
        srcRect.x = frame * frameWidth;
        lastFrameTime = currentTicks;
    }

    destRect.x = static_cast<int>(posX);
    destRect.y = static_cast<int>(posY);
}

void Rabbit::render(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0, NULL, flip);
}

