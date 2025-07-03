#pragma once
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstdio>
#include <emscripten.h>
#include "../functions/Consts.hpp"
#include "../enum/Types.hpp"
#include "Paddle.hpp"

// Forward declaration para evitar dependencias circulares
class Balls;

class Gifts {
public:
    Gifts() : texturesLoaded(false) {
    }
    
    ~Gifts() {
        if (texturesLoaded) {
            UnloadTexture(giftMultiBallTexture);
            UnloadTexture(giftPaddleExpandTexture);
        }
    }
    
    void loadTextures() {
        if (!texturesLoaded) {
            giftMultiBallTexture = LoadTexture("sprites/gift_multi_ball.png");
            giftPaddleExpandTexture = LoadTexture("sprites/gift_paddle_expand.png");
            texturesLoaded = true;
        }
    }
    
    struct Gift {
        Rectangle rect;
        bool active;
        GiftType type;
    };

    void add(Vector2 position, GiftType type) {
        gifts.push_back({
            position.x,
            position.y,
            (float)Consts::GIFT_SIZE,
            (float)Consts::GIFT_SIZE,
            true,
            type
        });
    }
    
    void update() {
        for (auto& gift : gifts) {
            gift.rect.y += Consts::GIFT_SPEED;
            if (gift.rect.y > Consts::WINDOW_HEIGHT) {
                gift.active = false;
            }
        }
    }
    
    void checkCollisions(Paddle* paddle, Balls* balls) {
        for (auto it = gifts.begin(); it != gifts.end();) {
            if (CheckCollisionRecs(paddle->getRect(), it->rect)) {
                if (it->type == GiftType::PADDLE_EXPAND) {
                    paddle->increaseWidth();
                } else if (it->type == GiftType::MULTI_BALL) {
                    addMultipleBalls(balls, 2);
                }
                *it = gifts.back();
                gifts.pop_back();
            } else {
                ++it;
            }
        }
    }

    void addMultipleBalls(Balls* balls, int count) {
        std::string debugInfo = "Pelotas agregadas:\\n";
        int ballsAdded = 0;
        
        auto originalBalls = balls->getBalls();
        
        for (const auto& ball : originalBalls) {
            if (ball.active) {
                for (int i = 0; i < count; i++) {
                    const float angle = (i + 1) * 0.3f * ((i % 2 == 0) ? 1 : -1);
                    float newVx = ball.vx * cos(angle) - ball.vy * sin(angle);
                    float newVy = ball.vx * sin(angle) + ball.vy * cos(angle);
                    balls->addBallAt(ball.rect.x, ball.rect.y, newVx, newVy);
                }
            }
        }
    }


    
    void show() {
        if (!texturesLoaded) {
            loadTextures();
        }
        
        for (const auto& gift : gifts) {
            if (gift.active) {
                Texture2D* textureToUse = &giftPaddleExpandTexture;
                
                if (gift.type == GiftType::MULTI_BALL) {
                    textureToUse = &giftMultiBallTexture;
                } else if (gift.type == GiftType::PADDLE_EXPAND) {
                    textureToUse = &giftPaddleExpandTexture;
                }
                
                Utils::DrawTextureScaled(*textureToUse, gift.rect.x, gift.rect.y, (float)Consts::GIFT_SIZE, (float)Consts::GIFT_SIZE, WHITE);
            }
        }
    }
    
    void reset() {
        gifts.clear();
    }
    
private:
    std::vector<Gift> gifts;
    bool texturesLoaded;
    
    // Texturas para los regalos
    Texture2D giftMultiBallTexture;
    Texture2D giftPaddleExpandTexture;
};