#pragma once
#include <raylib.h>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Ball.hpp"

class Paddle {
public:
    Paddle() {
        rect.width = Consts::PADDLE_W;
        rect.height = Consts::PADDLE_H;
    }
    void center() {
        rect.x = (Consts::WINDOW_WIDTH  - rect.width) / 2;
        rect.y = Consts::WINDOW_HEIGHT - 40;
    }
    void handleInput() {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) rect.x -= Consts::PADDLE_SPEED;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) rect.x += Consts::PADDLE_SPEED;
        if (rect.x < 0) rect.x = 0;
        if (rect.x + rect.width > Consts::WINDOW_WIDTH) rect.x = Consts::WINDOW_WIDTH - rect.width;
    }
    void checkCollisions(Ball* ball) {
        if (CheckCollisionRecs(ball->getRect(), rect)) {
            ball->rebound(rect);
        }
    }
    
    void show() {
        DrawRectangleRec(rect, WHITE);
    }
    void reset() {
        defaultSize();
        center();
    }

    void defaultSize() {
        rect.width = Consts::PADDLE_W;
    }

    void increaseWidth() {
        rect.width += Consts::PADDLE_INCREMENT_POINTS;
        if (rect.width > Consts::PADDLE_MAX_W) {
            rect.width = Consts::PADDLE_MAX_W;
        }
    }
    Rectangle &getRect()
    {
        return rect;
    }
private:
    Rectangle rect{};
};
