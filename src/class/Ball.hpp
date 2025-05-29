#pragma once
#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"

class Ball
{
public:
    Ball(SDL_Renderer *r) : renderer(r)
    {
        rect.w = rect.h = Consts::BALL_SIZE;
    }
    void center()
    {
        rect.x = (Consts::WINDOW_WIDTH - rect.w) / 2;
        rect.y = (Consts::WINDOW_HEIGHT - rect.h) / 2;
    }
    void startMovement()
    {
        vx = (std::rand() % 2 ? 1 : -1) * Consts::BALL_SPEED;
        vy = -Consts::BALL_SPEED;
    }
    void update()
    {
        rect.x += vx;
        rect.y += vy;

        if (rect.x <= 0 || rect.x + rect.w >= Consts::WINDOW_WIDTH)
            reverseX();
        if (rect.y <= 0)
            reverseY();
    }
    void reverseY()
    {
        vy = -vy;
    }
    void reverseX()
    {
        vx = -vx;
    }
    bool isOut() 
    {
        return rect.y + rect.h >= Consts::WINDOW_HEIGHT;
    }
    void show()
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    void reset() 
    {
        center();
        startMovement();
    }
    SDL_Rect &getRect()
    {
        return rect;
    }

private:
    SDL_Renderer *renderer;
    SDL_Rect rect{};
    float vx = 0, vy = 0;
};
