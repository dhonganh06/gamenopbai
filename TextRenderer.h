#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextRenderer {
public:
    TextRenderer(SDL_Renderer* renderer);
    ~TextRenderer();

    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void renderText(const std::string& text, int x, int y, SDL_Color color, int fontSize);
    bool showIntroAndGuide();

private:
    enum class ScreenState {
        START,
        INSTRUCTIONS
    };

    SDL_Renderer* renderer;
    TTF_Font* font;
    int defaultFontSize;
    ScreenState state;
    SDL_Rect instructionsRect;
    SDL_Rect playRect;


    bool instructionsHovered;
    bool playHovered;
    bool instructionsPressed;
    bool playPressed;

    void renderStartScreen();
    void renderInstructionsScreen();
};

#endif
