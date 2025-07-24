#pragma once
#include <raylib.h>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Balls.hpp"

class Paddle {
public:
    Paddle() {
        rect.width = Consts::PADDLE_W;
        rect.height = Consts::PADDLE_H;
        texturesLoaded = false;
    }
    
    ~Paddle() {
        if (texturesLoaded) {
            UnloadTexture(paddleTexture);
        }
    }
    
    void loadTextures() {
        if (!texturesLoaded) {
            paddleTexture = LoadTexture("sprites/paddle.png");
            texturesLoaded = true;
        }
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
    void checkCollisions(Balls* balls) {
        auto& originalBalls = balls->getBalls();
        for (auto& ballInstance : originalBalls) {
            if (ballInstance.active && CheckCollisionRecs(ballInstance.rect, rect)) {
                balls->rebound(rect, ballInstance);
            }
        }
    }
    
    void show() {
        if (!texturesLoaded) {
            loadTextures();
        }
        Utils::DrawTextureThreeSlice(paddleTexture, rect.x, rect.y, rect.width, rect.height, 20.0f);
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
    bool texturesLoaded;
    Texture2D paddleTexture;
};
