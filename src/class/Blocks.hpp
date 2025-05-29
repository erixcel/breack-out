#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Ball.hpp"

class Blocks
{
public:
    Blocks(SDL_Renderer *r, int rows, int cols) : renderer(r), rows(rows), cols(cols)
    {
        build();
    }
    void checkCollisions(Ball *ball)
    {
        const auto &ballRect = ball->getRect();
        for (auto it = bricks.begin(); it != bricks.end();)
        {
            if (Utils::checkCollision(ballRect, *it))
            {
                it = bricks.erase(it);
                ball->reverseY();
            }
            else
                ++it;
        }
    }
    bool isEmpty() const {
        return bricks.empty();
    }
    void show()
    {
        SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255);
        for (const auto &b : bricks)
            SDL_RenderFillRect(renderer, &b);
    }
    void reset()
    {
        build();
    }

private:
    SDL_Renderer *renderer;
    std::vector<SDL_Rect> bricks;
    int rows, cols;
    
    void build()
    {
        bricks.clear();
        int totalW = cols * Consts::BLOCK_W + (cols - 1) * Consts::BLOCK_GAP;
        int offsetX = (Consts::WINDOW_WIDTH - totalW) / 2;
        int offsetY = 60;
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col)
                bricks.push_back({offsetX + col * (Consts::BLOCK_W + Consts::BLOCK_GAP),
                                  offsetY + row * (Consts::BLOCK_H + Consts::BLOCK_GAP),
                                  Consts::BLOCK_W, Consts::BLOCK_H});
    }
};
