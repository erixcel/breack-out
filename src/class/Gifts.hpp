#pragma once
#include <raylib.h>
#include <vector>
#include "../functions/Consts.hpp"
#include "Paddle.hpp"

class Gifts {
public:
    struct Gift {
        Rectangle rect;
        bool active;
    };

    void add(Vector2 position) {
        gifts.push_back({
            position.x,
            position.y,
            (float)Consts::GIFT_SIZE,
            (float)Consts::GIFT_SIZE,
            true
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
    
    void checkCollisions(Paddle* paddle) {
        for (auto it = gifts.begin(); it != gifts.end();) {
            if (CheckCollisionRecs(paddle->getRect(), it->rect)) {
                paddle->increaseWidth();
                *it = gifts.back();
                gifts.pop_back();
            } else {
                ++it;
            }
        }
        for (auto& gift : gifts) {
            if (gift.active && CheckCollisionRecs(gift.rect, paddle->getRect())) {
                gift.active = false;
                paddle->increaseWidth();
            }
        }
    }
    
    void show() {
        for (const auto& gift : gifts) {
            if (gift.active) {
                DrawRectangleRec(gift.rect, GOLD);
            }
        }
    }
    
    void reset() {
        gifts.clear();
    }
    
private:
    std::vector<Gift> gifts;
};