#pragma once
#include <raylib.h>
#include <vector>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Ball.hpp"

class Blocks
{
public:
    Blocks(int rows, int cols) : rows(rows), cols(cols)
    {
        build();
    }

    void checkCollisions(Ball *ball)
    {
        for (auto it = bricks.begin(); it != bricks.end();){
            if (CheckCollisionRecs(ball->getRect(), *it)){
                ball->rebound(*it);
                *it = bricks.back();
                bricks.pop_back();
            } else {
                ++it;
            }
        }

    }
    bool isEmpty() 
    {
        return bricks.empty();
    }    
    void show()
    {
        for (const auto &b : bricks) {
            DrawRectangleRec(b, SKYBLUE);
        }            
    }
    void reset()
    {
        build();
    }

private:
    std::vector<Rectangle> bricks;
    int rows, cols;
      
    void build()
    {
        bricks.clear();
        int totalW = cols * Consts::BLOCK_W + (cols - 1) * Consts::BLOCK_GAP;
        int offsetX = (Consts::WINDOW_WIDTH - totalW) / 2;
        int offsetY = 60;
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col)
                bricks.push_back(
                    {
                        (float)(offsetX + col * (Consts::BLOCK_W + Consts::BLOCK_GAP)),
                        (float)(offsetY + row * (Consts::BLOCK_H + Consts::BLOCK_GAP)),
                        (float)Consts::BLOCK_W, 
                        (float)Consts::BLOCK_H,
                    }
                );
    }
};
