#include <raylib.h>
#include <emscripten.h>
#include "src/enum/State.hpp"
#include "src/functions/Consts.hpp"
#include "src/functions/Utils.hpp"
#include "src/class/Paddle.hpp"
#include "src/class/Ball.hpp"
#include "src/class/Blocks.hpp"

Paddle* paddle = nullptr;
Ball* ball = nullptr;
Blocks* blocks = nullptr;
EndState endState = EndState::NONE;
bool showEndModal = false;

void loop() {
    if (WindowShouldClose()) {
        CloseWindow();
        return;
    }

    if (!showEndModal) {
        ball->update();
        paddle->handleInput();
        paddle->checkCollisions(ball);
        blocks->checkCollisions(ball);

        if (blocks->isEmpty()) {
            showEndModal = true;
            endState = EndState::VICTORY;
        }
        if (ball->isOut()) {
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
                // Cerrar modal y reiniciar juego
                showEndModal = false;
                ball->reset();
                paddle->reset();
                blocks->reset();
                endState = EndState::NONE;
            }
        }
    }

    BeginDrawing();
    ClearBackground({0, 12, 33, 61});

    ball->show();
    blocks->show();
    paddle->show();

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

    ball = new Ball();
    paddle = new Paddle();
    blocks = new Blocks(5, 10);
    
    ball->reset();
    paddle->reset();
    blocks->reset();

    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
