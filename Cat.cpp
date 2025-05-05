#include "Cat.h"

Cat::Cat(SDL_Texture* tex, int x, int y, int screenWidth, int screenHeight)
    : texture(tex),
      frame(0),
      totalFrames(3),
      posX(x),
      posY(y),
      velocityX(0.0f),
      velocityY(0.0f),
      moveSpeed(4.0f),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      facingRight(true),
      jumping(false),
      gravity(0.3f),
      jumpStrength(-12.0f),
      lastFrameTime(SDL_GetTicks())
{
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    frameWidth = texW / totalFrames;
    frameHeight = texH;

    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {x, y, frameWidth, frameHeight};
}

void Cat::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
                velocityX = -moveSpeed;
                facingRight = false;
                break;
            case SDLK_d:
                velocityX = moveSpeed;
                facingRight = true;
                break;
            case SDLK_w:
                velocityY = -moveSpeed;
                break;
            case SDLK_s:
                velocityY = moveSpeed;
                break;
            case SDLK_e:
                if (!jumping) {
                    velocityY = jumpStrength;
                    jumping = true;
                }
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
            case SDLK_d:
                velocityX = 0.0f;
                break;
            case SDLK_w:
            case SDLK_s:
                velocityY = 0.0f;
                break;
        }
    }
}

void Cat::update() {
    if (jumping) velocityY += gravity;

    posX += velocityX;
    posY += velocityY;

    if (posX < 0) posX = 0;
    if (posX + destRect.w > screenWidth) posX = screenWidth - destRect.w;

    if (posY + destRect.h >= screenHeight) {
        posY = screenHeight - destRect.h;
        velocityY = 0.0f;
        jumping = false;
    } else if (posY < 0) {
        posY = 0;
        velocityY = 0.0f;
    }

    Uint32 currentTicks = SDL_GetTicks();
    if (currentTicks - lastFrameTime >= animationSpeed) {
        frame = (frame + 1) % totalFrames;
        srcRect.x = frame * frameWidth;
        lastFrameTime = currentTicks;
    }

    destRect.x = static_cast<int>(posX);
    destRect.y = static_cast<int>(posY);
}

void Cat::render(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0, NULL, flip);
}

SDL_Rect Cat::getBodyRect() {
    return destRect;
}

void Cat::checkWallCollisions(std::vector<Wall>& walls, int& catScore) {
    for (Wall& wall : walls) {
        if (wall.checkCollision(destRect)) {
            wall.active = false;
            catScore += 10;
        }
    }
}
