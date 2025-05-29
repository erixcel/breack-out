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
        originalWidth = Consts::PADDLE_W;
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
    
    void expand() {
        float oldCenterX = rect.x + rect.width / 2;
        rect.width = originalWidth * 1.5f; // Expandir 50%
        rect.x = oldCenterX - rect.width / 2; // Mantener centrado
        
        // Asegurar que no salga de los límites
        if (rect.x < 0) rect.x = 0;
        if (rect.x + rect.width > Consts::WINDOW_WIDTH) rect.x = Consts::WINDOW_WIDTH - rect.width;
    }
    
    void show() {
        DrawRectangleRec(rect, WHITE);
    }
    
    void reset() {
        rect.width = originalWidth;
        center();
    }
    
    Rectangle getRect() const {
        return rect;
    }
    
private:
    Rectangle rect{};
    float originalWidth;
};
