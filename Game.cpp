#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 30;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;
const int WIN_SCORE = 700;

SDL_Texture* Game::loadTexture(const char* filename, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture) {
        std::cerr << "Failed to load texture: " << filename << " | Error: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }
    return texture;
}

Game::Game() :
    window(nullptr),
    renderer(nullptr),
    background(nullptr),
    wallTexture(nullptr),
    birdTexture(nullptr),
    rabbitTexture(nullptr),
    catTexture(nullptr),
    endTexture(nullptr),
    bird(nullptr),
    rabbit(nullptr),
    cat(nullptr),
    textRenderer(nullptr),
    birdScore(0),
    catScore(0),
    running(true),
    gameEnded(false),
    winner(0)
{
    std::srand(std::time(nullptr));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Bird, Rabbit and Cat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    textRenderer = new TextRenderer(renderer);
    if (textRenderer->showIntroAndGuide()) {
        background = loadTexture("cutebaby.jpg", renderer);
        wallTexture = loadTexture("food.png", renderer);
        birdTexture = loadTexture("bird.png", renderer);
        rabbitTexture = loadTexture("rabbit.png", renderer);
        catTexture = loadTexture("cat.png", renderer);
        endTexture = loadTexture("end.jpg", renderer);

        bird = new Bird(birdTexture, 0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, SCREEN_HEIGHT);
        rabbit = new Rabbit(rabbitTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
        cat = new Cat(catTexture, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 70, SCREEN_WIDTH, SCREEN_HEIGHT);

        for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
            for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
                walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE, wallTexture);
            }
        }

        audioManager.loadMusic("music.ogg");
        audioManager.playMusic();
    } else {
        running = false;
    }
}

Game::~Game() {
    audioManager.stopMusic();

    if (background) SDL_DestroyTexture(background);
    if (wallTexture) SDL_DestroyTexture(wallTexture);
    if (birdTexture) SDL_DestroyTexture(birdTexture);
    if (rabbitTexture) SDL_DestroyTexture(rabbitTexture);
    if (catTexture) SDL_DestroyTexture(catTexture);
    if (endTexture) SDL_DestroyTexture(endTexture);

    delete bird;
    delete rabbit;
    delete cat;
    delete textRenderer;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {

            if (!gameEnded) {
                gameEnded = true;
                audioManager.stopMusic();
            } else {
                running = false;
            }
        }

        if (!gameEnded) {
            bird->handleEvent(e);
            cat->handleEvent(e);
        } else if (e.type == SDL_KEYDOWN) {
            running = false;
        }
    }
}

void Game::update() {
    if (gameEnded) return;

    bird->update();
    bird->checkWallCollisions(walls, birdScore);

    rabbit->update();

    cat->update();
    cat->checkWallCollisions(walls, catScore);

    SDL_Rect rabbitRect = rabbit->getBodyRect();
    SDL_Rect birdRect = bird->getBodyRect();
    SDL_Rect catRect = cat->getBodyRect();

    bool collideWithBird = SDL_HasIntersection(&rabbitRect, &birdRect);
    bool collideWithCat = SDL_HasIntersection(&rabbitRect, &catRect);


    if (collideWithBird || collideWithCat) {
        gameEnded = true;
        audioManager.stopMusic();

        if (collideWithBird) winner = 2;
        else if (collideWithCat) winner = 1;
    } else {

        if (birdScore + catScore >= WIN_SCORE) {
            gameEnded = true;
            audioManager.stopMusic();

            if (birdScore > catScore) winner = 1;
            else if (catScore > birdScore) winner = 2;
            else winner = 0;
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    if (gameEnded) {
        SDL_RenderCopy(renderer, endTexture, NULL, NULL);

        std::string resultText;

        if (winner == 1) resultText = "Bird wins!";
        else if (winner == 2) resultText = "Cat wins!";
        else resultText = "Draw!";

        textRenderer->renderText(resultText, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, {255, 0, 0, 255}, 40);
        textRenderer->renderText("Press any key to exit", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 50, {255, 255, 255, 255}, 24);
    } else {
        SDL_RenderCopy(renderer, background, NULL, NULL);

        for (auto& wall : walls) wall.render(renderer);

        bird->render(renderer);
        rabbit->render(renderer);
        cat->render(renderer);

        textRenderer->renderText("Bird: " + std::to_string(birdScore), 10, 10, {255, 0, 255, 255});

        std::string catScoreText = "Cat: " + std::to_string(catScore);
        int textWidth = catScoreText.length() * 10;
        textRenderer->renderText(catScoreText, SCREEN_WIDTH - textWidth - 10, 10, {0, 200, 255, 255});
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    while (running) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

