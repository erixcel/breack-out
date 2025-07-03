#pragma once
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "../functions/Consts.hpp"
#include "../enum/Types.hpp"
#include "Paddle.hpp"

// Forward declaration para evitar dependencias circulares
class Balls;

class Gifts {
public:
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
        for (const auto& ball : balls->getBalls()) {
            if (ball.active) {
                for (int i = 0; i < count; i++) {
                    float angleOffset = (i + 1) * 0.3f;
                    
                    float newVx = ball.vx + (std::rand() % 3 - 1) * 0.5f;
                    float newVy = ball.vy + (std::rand() % 3 - 1) * 0.5f;
                    
                    if (abs(newVx) < 2.0f) newVx = (newVx >= 0) ? 2.0f : -2.0f;
                    if (abs(newVy) < 2.0f) newVy = (newVy >= 0) ? 2.0f : -2.0f;
                    
                    balls->addBallAt(ball.rect.x, ball.rect.y, newVx, newVy);
                }
            }
        }
    }


    
    void show() {
        for (const auto& gift : gifts) {
            if (gift.active) {
                Color color = (gift.type == GiftType::PADDLE_EXPAND) ? GOLD : GREEN;
                DrawRectangleRec(gift.rect, color);
            }
        }
    }
    
    void reset() {
        gifts.clear();
    }
    
private:
    std::vector<Gift> gifts;
};