#include <raylib.h>
#include <emscripten.h>
#include <vector>
#include "src/enum/State.hpp"
#include "src/functions/Consts.hpp"
#include "src/functions/Utils.hpp"
#include "src/class/Paddle.hpp"
#include "src/class/Ball.hpp"
#include "src/class/Blocks.hpp"
#include "src/class/ItemManager.hpp"

Paddle* paddle = nullptr;
std::vector<Ball> balls;
Blocks* blocks = nullptr;
ItemManager* itemManager = nullptr;
EndState endState = EndState::NONE;
bool showEndModal = false;

void loop() {
    if (WindowShouldClose()) {
        CloseWindow();
        return;
    }

    if (!showEndModal) {
        // Actualizar todas las pelotas
        for (auto& ball : balls) {
            ball.update();
            paddle->checkCollisions(&ball);
            blocks->checkCollisions(&ball, itemManager);
        }
        
        // Actualizar items
        itemManager->update();
        itemManager->checkPaddleCollisions(paddle, &balls);
        
        paddle->handleInput();

        if (blocks->isEmpty()) {
            showEndModal = true;
            endState = EndState::VICTORY;
        }
        
        // Verificar si todas las pelotas están fuera
        bool allBallsOut = true;
        for (const auto& ball : balls) {
            if (!ball.isOut()) {
                allBallsOut = false;
                break;
            }
        }
        
        if (allBallsOut) {
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
            
            // Verificar si el clic está dentro del botón            if (mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
                mousePos.y >= buttonY && mousePos.y <= buttonY + buttonHeight) {
                // Cerrar modal y reiniciar juego
                showEndModal = false;
                
                // Reiniciar con una sola pelota
                balls.clear();
                Ball newBall;
                newBall.reset();
                balls.push_back(newBall);
                
                paddle->reset();
                blocks->reset();
                itemManager->reset();
                endState = EndState::NONE;
            }
        }
    }    BeginDrawing();
    ClearBackground({0, 12, 33, 61});

    // Dibujar todas las pelotas
    for (auto& ball : balls) {
        ball.show();
    }
    
    blocks->show();
    paddle->show();
    itemManager->show();

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
    blocks = new Blocks(5, 10);
    itemManager = new ItemManager();
    
    // Inicializar con una pelota
    Ball initialBall;
    initialBall.reset();
    balls.push_back(initialBall);
    
    paddle->reset();
    blocks->reset();

    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
