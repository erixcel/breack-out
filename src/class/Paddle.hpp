#pragma once
#include <SDL2/SDL.h>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Ball.hpp"

class Paddle {
public:
    Paddle(SDL_Renderer* r) : renderer(r) {
        rect.w = Consts::PADDLE_W;
        rect.h = Consts::PADDLE_H;
    }
    void center() {
        rect.x = (Consts::WINDOW_WIDTH  - rect.w) / 2;
        rect.y = Consts::WINDOW_HEIGHT - 40;
    }
    void handleInput(const Uint8* keys) {
        if (keys[SDL_SCANCODE_LEFT]  || keys[SDL_SCANCODE_A]) rect.x -= Consts::PADDLE_SPEED;
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) rect.x += Consts::PADDLE_SPEED;
        if (rect.x < 0) rect.x = 0;
        if (rect.x + rect.w > Consts::WINDOW_WIDTH) rect.x = Consts::WINDOW_WIDTH - rect.w;
    }
    void checkCollisions(Ball* ball) {
        const auto& ballRect = ball->getRect();
        if (Utils::checkCollision(ballRect, rect)) {
            ball->reverseY();
        }
    }
    void show() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    void reset() {
        center();
    }
private:
    SDL_Renderer* renderer;
    SDL_Rect rect{};
};
