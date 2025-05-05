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
      jumpStrength(-25.0f),
      lastFrameTime(SDL_GetTicks())
{
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    frameWidth = texW / totalFrames;
    frameHeight = texH;

    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {x, y, frameWidth, frameHeight};
}

