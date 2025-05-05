
#include "Wall.h"

Wall::Wall(int startX, int startY, SDL_Texture* tex) : active(true), texture(tex) {
    rect = {startX, startY, 40, 40};
}

void Wall::render(SDL_Renderer* renderer) {
    if (active && texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

bool Wall::checkCollision(const SDL_Rect& otherRect) {
    if (!active) return false;
    return SDL_HasIntersection(&rect, &otherRect);
}

