#include <raylib.h>
#include <emscripten.h>
#include "src/enum/State.hpp"
#include "src/functions/Consts.hpp"
#include "src/functions/Utils.hpp"
#include "src/class/Paddle.hpp"
#include "src/class/Balls.hpp"
#include "src/class/Blocks.hpp"
#include "src/class/Gifts.hpp"
#include "src/class/Walls.hpp"

Paddle* paddle = nullptr;
Balls* balls = nullptr;
Blocks* blocks = nullptr;
Gifts* gifts = nullptr;
Walls* walls = nullptr;
EndState endState = EndState::NONE;
bool showEndModal = false;

void loop() {
    if (WindowShouldClose()) {
        CloseWindow();
        return;
    }

    if (!showEndModal) {
        balls->update();
        gifts->update();
        paddle->handleInput();
        paddle->checkCollisions(balls);
        blocks->checkCollisions(balls, gifts);
        walls->checkCollisions(balls);
        gifts->checkCollisions(paddle, balls);

        if (blocks->isEmpty()) {
            showEndModal = true;
            endState = EndState::VICTORY;
        }
        if (balls->isOut()) {
            showEndModal = true;
            endState = EndState::DEFEAT;
        }
    } else {
        // Detectar clic en el botón "Continuar" del modal
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            int modalWidth = 400;
            int modalHeight = 200;
            int modalX = (Consts::WINDOW_WIDTH - modalWidth) / 2;
            int modalY = (Consts::WINDOW_HEIGHT - modalHeight) / 2;
            
            int buttonWidth = 120;
            int buttonHeight = 40;
            int buttonX = modalX + (modalWidth - buttonWidth) / 2;
            int buttonY = modalY + modalHeight - 60;
            
            // Verificar si el clic está dentro del botón
            if (mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
                mousePos.y >= buttonY && mousePos.y <= buttonY + buttonHeight) {
                    
                showEndModal = false;
                balls->reset();
                paddle->reset();
                blocks->reset();
                gifts->reset();
                walls->reset();
                endState = EndState::NONE;
            }
        }
    }

    BeginDrawing();
    ClearBackground({0, 12, 33, 61});

    balls->show();
    blocks->show();
    walls->show();
    paddle->show();
    gifts->show();

    if (showEndModal) {
        if (endState == EndState::VICTORY) {
            Utils::showVictoryModal();
        } else {
            Utils::showDefeatModal();
        }
    }
    
    EndDrawing();
}

int main() {
    InitWindow(Consts::WINDOW_WIDTH, Consts::WINDOW_HEIGHT, "Minimal Breakout");

    paddle = new Paddle();
    gifts = new Gifts();
    balls = new Balls();
    walls = new Walls(2, 25);
    blocks = new Blocks(5, 10);
    
    paddle->reset();
    balls->reset();
    blocks->reset();
    walls->reset();

    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}