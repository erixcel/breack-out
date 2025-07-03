#pragma once
#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "../enum/Types.hpp"
#include "Balls.hpp"
#include "Gifts.hpp"

struct Block {
    Rectangle rect;
    bool hasGift;
    GiftType giftType;
};

class Blocks {
public:
    Blocks(int rows, int cols) : rows(rows), cols(cols) {
        build();
    }

    void checkCollisions(Balls* balls, Gifts* gifts) {
        for (auto& ball : balls->getBalls()) {
            if (!ball.active) continue;
            
            for (auto it = blocks.begin(); it != blocks.end();) {
                if (CheckCollisionRecs(ball.rect, it->rect)) {
                    balls->rebound(it->rect, ball);
                    
                    if (it->hasGift) {
                        Vector2 pos = {
                            it->rect.x + it->rect.width/2 - Consts::GIFT_SIZE/2,
                            it->rect.y + it->rect.height
                        };
                        gifts->add(pos, it->giftType);
                    }
                    
                    *it = blocks.back();
                    blocks.pop_back();
                    break;
                } else {
                    ++it;
                }
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
                
                // Si tiene regalo, decidir el tipo aleatoriamente
                if (block.hasGift) {
                    float gift_type_random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                    // 60% probabilidad de regalo dorado (expande paleta), 40% verde (multi-pelota)
                    block.giftType = (gift_type_random < 0.6f) ? GiftType::PADDLE_EXPAND : GiftType::MULTI_BALL;
                } else {
                    block.giftType = GiftType::PADDLE_EXPAND; // Valor por defecto (no se usa si no hay regalo)
                }

                blocks.push_back(block);
            }
        }
    }
};