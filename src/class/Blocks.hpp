#pragma once
#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Ball.hpp"
#include "Gifts.hpp"

struct Block {
    Rectangle rect;
    bool hasGift;
};

class Blocks {
public:
    Blocks(int rows, int cols) : rows(rows), cols(cols) {
        build();
    }

    void checkCollisions(Ball* ball, Gifts& gifts) {
        for (auto it = blocks.begin(); it != blocks.end();) {
            if (CheckCollisionRecs(ball->getRect(), it->rect)) {
                ball->rebound(it->rect);
                
                if (it->hasGift) {
                    Vector2 pos = {
                        it->rect.x + it->rect.width/2 - Consts::GIFT_SIZE/2,
                        it->rect.y + it->rect.height
                    };
                    gifts.add(pos);
                }
                
                *it = blocks.back();
                blocks.pop_back();
            } else {
                ++it;
            }
        }
    }
    
    bool isEmpty() {
        return blocks.empty();
    }
    
    void show() {
        for (const auto& b : blocks) {
            DrawRectangleRec(b.rect, b.hasGift ? PURPLE : SKYBLUE);
        }
    }
    
    void reset() {
        build();
    }

private:
    std::vector<Block> blocks;
    int rows, cols;
      
    void build() {
        blocks.clear();
        int totalW = cols * Consts::BLOCK_W + (cols - 1) * Consts::BLOCK_GAP;
        int offsetX = (Consts::WINDOW_WIDTH - totalW) / 2;
        int offsetY = 60;
        
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                Block block;
                block.rect = {
                    (float)(offsetX + col * (Consts::BLOCK_W + Consts::BLOCK_GAP)),
                    (float)(offsetY + row * (Consts::BLOCK_H + Consts::BLOCK_GAP)),
                    (float)Consts::BLOCK_W, 
                    (float)Consts::BLOCK_H
                };
                
                float random_value = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                block.hasGift = (random_value < Consts::GIFT_PROBABILITY);

                blocks.push_back(block);
            }
        }
    }
};