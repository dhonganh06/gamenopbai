#include "TextRenderer.h"
#include <iostream>
#include <SDL_image.h>

TextRenderer::TextRenderer(SDL_Renderer* renderer)
    : renderer(renderer),
      font(nullptr),
      defaultFontSize(24),
      state(ScreenState::START),
      instructionsHovered(false),
      playHovered(false),
      instructionsPressed(false),
      playPressed(false) {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
        return;
    }

    font = TTF_OpenFont("fonts/WinkySans-VariableFont_wght.ttf", defaultFontSize);
    if (!font) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
    }

    instructionsRect = {200, 300, 200, 50};
    playRect = {450, 300, 150, 50};
}

TextRenderer::~TextRenderer() {
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    renderText(text, x, y, color, defaultFontSize);
}

void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    if (!font || !renderer) return;

    if (TTF_FontHeight(font) != fontSize) {
        TTF_CloseFont(font);
        font = TTF_OpenFont("fonts/WinkySans-VariableFont_wght.ttf", fontSize);
        if (!font) {
            std::cerr << "Failed to load font with size " << fontSize << "! Error: " << TTF_GetError() << std::endl;
            return;
        }
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to create text surface! Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create text texture! Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void TextRenderer::renderStartScreen() {
    SDL_RenderClear(renderer);

    SDL_Texture* background = IMG_LoadTexture(renderer, "choo.jpg");
    if (!background) {
        std::cerr << "Failed to load background: " << IMG_GetError() << std::endl;
        return;
    }
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_DestroyTexture(background);

    SDL_Color white = {255, 255, 255, 255};

    renderText("WELCOME TO MY GAME!", 50, 50, white, 28);


    if (instructionsPressed) {
        SDL_SetRenderDrawColor(renderer, 199, 21, 133, 255);
    } else if (instructionsHovered) {
        SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    }
    SDL_RenderFillRect(renderer, &instructionsRect);
    renderText("Instructions", instructionsRect.x + 20, instructionsRect.y + 15, white, 20);


    if (playPressed) {
        SDL_SetRenderDrawColor(renderer, 199, 21, 133, 255);
    } else if (playHovered) {
        SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    }
    SDL_RenderFillRect(renderer, &playRect);
    renderText("Play", playRect.x + 40, playRect.y + 15, white, 20);

    SDL_RenderPresent(renderer);
}

void TextRenderer::renderInstructionsScreen() {
    SDL_RenderClear(renderer);

    SDL_Texture* background = IMG_LoadTexture(renderer, "choo.jpg");
    if (!background) {
        std::cerr << "Failed to load background: " << IMG_GetError() << std::endl;
        return;
    }
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_DestroyTexture(background);

    SDL_Color white = {255, 255, 255, 255};

    renderText("INSTRUCTIONS:", 50, 100, white, 24);
    renderText("1. Bird: Arrow Keys + SPACE to jump", 50, 150, white, 20);
    renderText("2. Cat: WASD + E to jump", 50, 200, white, 20);
    renderText("3. Collect chickens to score points!", 50, 250, white, 20);
    renderText("Press Enter to go back...", 50, 300, white, 18);

    SDL_RenderPresent(renderer);
}

bool TextRenderer::showIntroAndGuide() {
    bool startGame = false;

    while (!startGame) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true;
            }

            if (state == ScreenState::START) {
                if (e.type == SDL_MOUSEMOTION) {
                    int mouseX = e.motion.x;
                    int mouseY = e.motion.y;

                    instructionsHovered = (mouseX >= instructionsRect.x &&
                                           mouseX <= instructionsRect.x + instructionsRect.w &&
                                           mouseY >= instructionsRect.y &&
                                           mouseY <= instructionsRect.y + instructionsRect.h);


                    playHovered = (mouseX >= playRect.x &&
                                   mouseX <= playRect.x + playRect.w &&
                                   mouseY >= playRect.y &&
                                   mouseY <= playRect.y + playRect.h);
                }


                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;


                    if (mouseX >= instructionsRect.x && mouseX <= instructionsRect.x + instructionsRect.w &&
                        mouseY >= instructionsRect.y && mouseY <= instructionsRect.y + instructionsRect.h) {
                        instructionsPressed = true;
                    }


                    if (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
                        mouseY >= playRect.y && mouseY <= playRect.y + playRect.h) {
                        playPressed = true;
                    }
                }


                if (e.type == SDL_MOUSEBUTTONUP) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;

                    if (mouseX >= instructionsRect.x && mouseX <= instructionsRect.x + instructionsRect.w &&
                        mouseY >= instructionsRect.y && mouseY <= instructionsRect.y + instructionsRect.h) {
                        if (instructionsPressed) {
                            state = ScreenState::INSTRUCTIONS;
                        }
                    }


                    if (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
                        mouseY >= playRect.y && mouseY <= playRect.y + playRect.h) {
                        if (playPressed) {
                            return true;
                        }
                    }


                    instructionsPressed = false;
                    playPressed = false;
                }
            } else if (state == ScreenState::INSTRUCTIONS) {
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                    state = ScreenState::START;
                }
            }
        }

        if (state == ScreenState::START) {
            renderStartScreen();
        } else if (state == ScreenState::INSTRUCTIONS) {
            renderInstructionsScreen();
        }
    }

    return true;
}
