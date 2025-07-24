#pragma once
#include <raylib.h>
#include <string>
#include <ctime>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"

class ScoreManager {
public:
    ScoreManager() {
        highScore = loadHighScore();
        reset();
    }
    
    void reset() {
        blocksDestroyed = 0;
        giftsCollected = 0;
        startTime = GetTime();
        gameTime = 0.0f;
        gameEnded = false;
    }
    
    void addBlockDestroyed() {
        blocksDestroyed++;
    }
    
    void addGiftCollected() {
        giftsCollected++;
    }
    
    void endGame() {
        if (!gameEnded) {
            gameTime = GetTime() - startTime;
            gameEnded = true;
            
            // Verificar y actualizar high score
            int currentScore = calculateTotalScore();
            if (currentScore > highScore) {
                highScore = currentScore;
                saveHighScore(highScore);
            }
        }
    }
    
    void update() {
        if (!gameEnded) {
            gameTime = GetTime() - startTime;
        }
    }
    
    void showGameTimeHUD() {
        std::string timeText = "Tiempo: " + formatTime(gameTime);
        DrawText(timeText.c_str(), 10, 10, 20, WHITE);
        
        std::string blocksText = "Bloques: " + std::to_string(blocksDestroyed);
        DrawText(blocksText.c_str(), 10, 35, 20, WHITE);
        
        std::string giftsText = "Gifts: " + std::to_string(giftsCollected);
        DrawText(giftsText.c_str(), 10, 60, 20, WHITE);
        
        std::string highScoreText = "Mejor: " + std::to_string(highScore);
        DrawText(highScoreText.c_str(), 10, 85, 20, GOLD);
    }
    
    void showFinalScore(bool victory) {
        // Configuración del modal
        int modalWidth = 500;
        int modalHeight = 350;
        int modalX = (Consts::WINDOW_WIDTH - modalWidth) / 2;
        int modalY = (Consts::WINDOW_HEIGHT - modalHeight) / 2;
        
        // Fondo semi-transparente
        DrawRectangle(0, 0, Consts::WINDOW_WIDTH, Consts::WINDOW_HEIGHT, Fade(BLACK, 0.5f));
        
        // Fondo del modal
        DrawRectangle(modalX, modalY, modalWidth, modalHeight, WHITE);
        DrawRectangleLines(modalX, modalY, modalWidth, modalHeight, BLACK);
        
        // Título
        std::string title = victory ? "¡VICTORIA!" : "DERROTA";
        int titleFontSize = 28;
        Vector2 titleSize = MeasureTextEx(GetFontDefault(), title.c_str(), titleFontSize, 1);
        int titleX = modalX + (modalWidth - titleSize.x) / 2;
        int titleY = modalY + 20;
        Color titleColor = victory ? DARKGREEN : RED;
        DrawText(title.c_str(), titleX, titleY, titleFontSize, titleColor);
        
        // Línea separadora
        DrawLine(modalX + 20, titleY + 40, modalX + modalWidth - 20, titleY + 40, LIGHTGRAY);
        
        // Estadísticas
        int statsY = titleY + 60;
        int lineHeight = 30;
        int statsFontSize = 18;
        
        // Tiempo de juego
        std::string timeText = "Tiempo de juego: " + formatTime(gameTime);
        DrawText(timeText.c_str(), modalX + 30, statsY, statsFontSize, DARKBLUE);
        
        // Bloques destruidos
        std::string blocksText = "Bloques destruidos: " + std::to_string(blocksDestroyed);
        DrawText(blocksText.c_str(), modalX + 30, statsY + lineHeight, statsFontSize, DARKBLUE);
        
        // Gifts recolectados
        std::string giftsText = "Gifts recolectados: " + std::to_string(giftsCollected);
        DrawText(giftsText.c_str(), modalX + 30, statsY + lineHeight * 2, statsFontSize, DARKBLUE);
        
        // Puntaje total (fórmula simple)
        int totalScore = calculateTotalScore();
        std::string scoreText = "Puntaje Total: " + std::to_string(totalScore);
        DrawText(scoreText.c_str(), modalX + 30, statsY + lineHeight * 3 + 10, 20, GOLD);
        
        // High Score
        std::string highScoreText = "Mejor Puntaje: " + std::to_string(highScore);
        Color highScoreColor = (totalScore > highScore) ? GREEN : PURPLE;
        DrawText(highScoreText.c_str(), modalX + 30, statsY + lineHeight * 4 + 10, 18, highScoreColor);
        
        // Nuevo récord
        if (totalScore > highScore && totalScore == calculateTotalScore()) {
            std::string newRecordText = "¡NUEVO RÉCORD!";
            DrawText(newRecordText.c_str(), modalX + 30, statsY + lineHeight * 5 + 10, 16, RED);
        }
        
        // Línea separadora
        DrawLine(modalX + 20, statsY + lineHeight * 6 + 20, modalX + modalWidth - 20, statsY + lineHeight * 6 + 20, LIGHTGRAY);
        
        // Botón "Continuar" - Actualizar coordenadas internas
        buttonRect.width = 120;
        buttonRect.height = 40;
        buttonRect.x = modalX + (modalWidth - buttonRect.width) / 2;
        buttonRect.y = modalY + modalHeight - 60;
        
        DrawRectangle(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, LIGHTGRAY);
        DrawRectangleLines(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, BLACK);
        
        const char* buttonText = "Continuar";
        int buttonFontSize = 16;
        Vector2 buttonTextSize = MeasureTextEx(GetFontDefault(), buttonText, buttonFontSize, 1);
        int buttonTextX = buttonRect.x + (buttonRect.width - buttonTextSize.x) / 2;
        int buttonTextY = buttonRect.y + (buttonRect.height - buttonTextSize.y) / 2;
        DrawText(buttonText, buttonTextX, buttonTextY, buttonFontSize, BLACK);
    }
    
    // Getters para acceso externo
    int getBlocksDestroyed() const { return blocksDestroyed; }
    int getGiftsCollected() const { return giftsCollected; }
    float getGameTime() const { return gameTime; }
    int getTotalScore() const { return calculateTotalScore(); }
    int getHighScore() const { return highScore; }
    Rectangle getButtonRect() const { return buttonRect; }
    
    // Método para verificar si se hizo clic en el botón
    bool isButtonClicked(Vector2 mousePos) const {
        return CheckCollisionPointRec(mousePos, buttonRect);
    }
    
private:
    int blocksDestroyed;
    int giftsCollected;
    double startTime;
    float gameTime;
    bool gameEnded;
    int highScore;
    Rectangle buttonRect; // Coordenadas del botón "Continuar"
    
    std::string formatTime(float seconds) {
        int minutes = (int)seconds / 60;
        int secs = (int)seconds % 60;
        int centiseconds = (int)((seconds - (int)seconds) * 100);
        
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%02d:%02d.%02d", minutes, secs, centiseconds);
        return std::string(buffer);
    }
    
    int calculateTotalScore() const {
        // Fórmula de puntaje: bloques * 100 + gifts * 50 + bonus por tiempo
        int baseScore = blocksDestroyed * 100 + giftsCollected * 50;
        
        // Bonus por tiempo (más puntos si terminas rápido)
        int timeBonus = 0;
        if (gameTime < 30.0f) timeBonus = 500;
        else if (gameTime < 60.0f) timeBonus = 300;
        else if (gameTime < 120.0f) timeBonus = 100;
        
        return baseScore + timeBonus;
    }
    
    int loadHighScore() {
#ifdef __EMSCRIPTEN__
        // Usar localStorage del navegador para cargar el high score
        char* result = (char*)EM_ASM_PTR({
            var highScore = localStorage.getItem('breakout_highscore');
            if (highScore === null) {
                return 0;
            }
            var str = highScore.toString();
            var len = lengthBytesUTF8(str) + 1;
            var ptr = _malloc(len);
            stringToUTF8(str, ptr, len);
            return ptr;
        });
        
        if (result == 0) {
            return 0;
        }
        
        int score = atoi(result);
        free(result);
        return score;
#else
        // Fallback para compilación nativa (retorna 0)
        return 0;
#endif
    }
    
    void saveHighScore(int score) {
#ifdef __EMSCRIPTEN__
        // Usar localStorage del navegador para guardar el high score
        EM_ASM({
            localStorage.setItem('breakout_highscore', $0.toString());
        }, score);
#endif
        // No hacer nada en compilación nativa
    }
};