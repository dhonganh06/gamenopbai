#include "Bird.h"

Bird::Bird(SDL_Texture* tex, int x, int y, int screenWidth, int screenHeight)
    : texture(tex),
      frame(0),
      totalFrames(5),
      posX(x),
      posY(y),
      velocityX(0.0f),
      velocityY(0.0f),
      gravity(0.5f),
      flapStrength(-8.0f),
      moveSpeed(5.0f),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      facingRight(true),
      lastFrameTime(SDL_GetTicks())
{
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    frameWidth = texW / totalFrames;
    frameHeight = texH;

    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {x, y, frameWidth / 2, frameHeight / 2};

    beakRect = {x + destRect.w - 10, y, 10, 10};
}

void Bird::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_SPACE:
                velocityY = flapStrength;
                break;
            case SDLK_UP:
                velocityY = -moveSpeed;
                break;
            case SDLK_DOWN:
                velocityY = moveSpeed;
                break;
            case SDLK_LEFT:
                velocityX = -moveSpeed;
                facingRight = false;
                break;
            case SDLK_RIGHT:
                velocityX = moveSpeed;
                facingRight = true;
                break;
        }
    }
    else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
                velocityY = 0.0f;
                break;
            case SDLK_LEFT:
            case SDLK_RIGHT:
                velocityX = 0.0f;
                break;
        }
    }
}

void Bird::update() {
    velocityY += gravity;
    posX += velocityX;
    posY += velocityY;

    if (posX < 0) posX = 0;
    if (posX + destRect.w > screenWidth) posX = screenWidth - destRect.w;
    if (posY < 0) posY = 0;
    if (posY + destRect.h > screenHeight) posY = screenHeight - destRect.h;

    Uint32 currentTicks = SDL_GetTicks();
    if (currentTicks - lastFrameTime >= animationSpeed) {
        frame = (frame + 1) % totalFrames;
        srcRect.x = frame * frameWidth;
        srcRect.y = 0;
        lastFrameTime = currentTicks;
    }

    destRect.x = static_cast<int>(posX);
    destRect.y = static_cast<int>(posY);

    if (facingRight) {
        beakRect.x = destRect.x + destRect.w - 10;
    } else {
        beakRect.x = destRect.x;
    }
    beakRect.y = destRect.y;
}

void Bird::render(SDL_Renderer* renderer) {
    double angle = velocityY * 0.0;

    if (angle > 30.0) angle = 30.0;
    if (angle < -30.0) angle = -30.0;

    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, NULL, flip);
}

void Bird::checkWallCollisions(std::vector<Wall>& walls, int& birdScore) {
    for (Wall& wall : walls) {
        if (wall.checkCollision(beakRect)) {
            wall.active = false;
            birdScore += 10;
        }
    }
}

SDL_Rect Bird::getBodyRect() {
    return destRect;
}


