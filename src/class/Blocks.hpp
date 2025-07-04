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
    Blocks(int rows, int cols) : rows(rows), cols(cols), texturesLoaded(false) {
        build();
    }
    
    ~Blocks() {
        if (texturesLoaded) {
            UnloadTexture(blockTexture);
            UnloadTexture(blockMultiBallTexture);
            UnloadTexture(blockPaddleExpandTexture);
        }
    }
    
    void loadTextures() {
        if (!texturesLoaded) {
            blockTexture = LoadTexture("sprites/block.png");
            blockMultiBallTexture = LoadTexture("sprites/block_multi_ball.png");
            blockPaddleExpandTexture = LoadTexture("sprites/block_paddle_expand.png");
            texturesLoaded = true;
        }
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
        if (!texturesLoaded) {
            loadTextures();
        }
        
        for (const auto& block : blocks) {
            Texture2D* textureToUse = &blockTexture;
            
            if (block.hasGift) {
                if (block.giftType == GiftType::MULTI_BALL) {
                    textureToUse = &blockMultiBallTexture;
                } else if (block.giftType == GiftType::PADDLE_EXPAND) {
                    textureToUse = &blockPaddleExpandTexture;
                }
            }
            
            Utils::DrawTextureScaled(*textureToUse, block.rect.x, block.rect.y, (float)Consts::BLOCK_W, (float)Consts::BLOCK_H, WHITE);
        }
    }
    
    void reset() {
        build();
    }

private:
    std::vector<Block> blocks;
    int rows, cols;
    bool texturesLoaded;
    
    // Texturas para los bloques
    Texture2D blockTexture;
    Texture2D blockMultiBallTexture;
    Texture2D blockPaddleExpandTexture;
      
    void build() {
        blocks.clear();
        int totalW = cols * Consts::BLOCK_W + (cols - 1) * Consts::BLOCK_GAP;
        int offsetX = (Consts::WINDOW_WIDTH - totalW) / 2;
        int offsetY = Consts::BLOCK_START_Y;
        
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
                
                if (block.hasGift) {
                    float gift_type_random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                    block.giftType = (gift_type_random < 0.5f) ? GiftType::PADDLE_EXPAND : GiftType::MULTI_BALL;
                } else {
                    block.giftType = GiftType::PADDLE_EXPAND;
                }

                blocks.push_back(block);
            }
        }
    }
};