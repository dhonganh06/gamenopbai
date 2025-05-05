#ifndef WALL_H
#define WALL_H

#include <SDL.h>
#include <SDL_image.h>

class Wall {
public:
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;

    Wall(int startX, int startY, SDL_Texture* tex);
    void render(SDL_Renderer* renderer);
    bool checkCollision(const SDL_Rect& otherRect);
};

#endif
