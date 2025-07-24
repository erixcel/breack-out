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
#include "src/class/ScoreManager.hpp"

Paddle* paddle = nullptr;
Balls* balls = nullptr;
Blocks* blocks = nullptr;
Gifts* gifts = nullptr;
Walls* walls = nullptr;
ScoreManager* scoreManager = nullptr;
EndState endState = EndState::NONE;
bool showEndModal = false;

void loop() {
    if (WindowShouldClose()) {
        CloseWindow();
        return;
    }

    if (!showEndModal) {
        scoreManager->update();
        balls->update();
        gifts->update();
        paddle->handleInput();
        paddle->checkCollisions(balls);
        blocks->checkCollisions(balls, gifts, scoreManager);
        walls->checkCollisions(balls);
        gifts->checkCollisions(paddle, balls, scoreManager);

        if (blocks->isEmpty()) {
            scoreManager->endGame();
            showEndModal = true;
            endState = EndState::VICTORY;
        }
        if (balls->isOut()) {
            scoreManager->endGame();
            showEndModal = true;
            endState = EndState::DEFEAT;
        }
    } else {
        // Detectar clic en el botón "Continuar" usando las coordenadas dinámicas
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            
            if (scoreManager->isButtonClicked(mousePos)) {
                showEndModal = false;
                balls->reset();
                paddle->reset();
                blocks->reset();
                gifts->reset();
                walls->reset();
                scoreManager->reset();
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
    
    // Mostrar HUD con estadísticas en tiempo real
    if (!showEndModal) {
        scoreManager->showGameTimeHUD();
    }

    if (showEndModal) {
        scoreManager->showFinalScore(endState == EndState::VICTORY);
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
    scoreManager = new ScoreManager();
    
    paddle->reset();
    balls->reset();
    blocks->reset();
    walls->reset();

    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}