#pragma once
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"

class Balls {
public:
    struct Ball {
        Rectangle rect;
        float vx, vy;
        bool active;
        bool stuckToPaddle;
        float paddleOffset;
        float previewVx, previewVy;
    };

    Balls() : texturesLoaded(false) {
        addBall();
    }
    
    ~Balls() {
        if (texturesLoaded) {
            UnloadTexture(ballTexture);
        }
    }
    
    void loadTextures() {
        if (!texturesLoaded) {
            ballTexture = LoadTexture("sprites/ball.png");
            texturesLoaded = true;
        }
    }
    
    void addBall() {
        Ball newBall;
        newBall.rect.width = newBall.rect.height = Consts::BALL_SIZE;
        newBall.vx = 0;
        newBall.vy = 0;
        newBall.active = true;
        newBall.stuckToPaddle = false;
        newBall.paddleOffset = 0;
        newBall.previewVx = 0;
        newBall.previewVy = 0;
        center(newBall);
        startMovement(newBall);
        balls.push_back(newBall);
    }
    
    void addBallAt(float x, float y, float vx, float vy) {
        Ball newBall;
        newBall.rect.width = newBall.rect.height = Consts::BALL_SIZE;
        newBall.rect.x = x;
        newBall.rect.y = y;
        newBall.vx = vx;
        newBall.vy = vy;
        newBall.active = true;
        newBall.stuckToPaddle = false;
        newBall.paddleOffset = 0;
        newBall.previewVx = 0;
        newBall.previewVy = 0;
        balls.push_back(newBall);
    }
    
    void center(Ball& ball) {
        ball.rect.x = (Consts::WINDOW_WIDTH - ball.rect.width) / 2;
        ball.rect.y = (Consts::WINDOW_HEIGHT - ball.rect.height) / 2;
    }
    
    void centerOnPaddle(Ball& ball, const Rectangle& paddleRect) {
        // Posicionar la pelota en el centro horizontal del paddle
        ball.rect.x = paddleRect.x + (paddleRect.width - ball.rect.width) / 2;
        // Posicionar la pelota justo encima del paddle
        ball.rect.y = paddleRect.y - ball.rect.height;
    }
    
    void startMovement(Ball& ball) {
        ball.vx = (std::rand() % 2 ? 1 : -1) * Consts::BALL_SPEED;
        ball.vy = -Consts::BALL_SPEED;
    }
    
    void update() {
        for (auto& ball : balls) {
            if (!ball.active) continue;
            
            if (ball.stuckToPaddle) continue;
            
            ball.rect.x += ball.vx;
            ball.rect.y += ball.vy;

            if (ball.rect.x <= 0 || ball.rect.x + ball.rect.width >= Consts::WINDOW_WIDTH)
                reverseX(ball);
            if (ball.rect.y <= 0)
                reverseY(ball);
                
            if (ball.rect.y + ball.rect.height >= Consts::WINDOW_HEIGHT) {
                ball.active = false;
            }
        }
        
        balls.erase(std::remove_if(balls.begin(), balls.end(), 
            [](const Ball& ball) { return !ball.active; }), balls.end());
    }
    
    void updateStuckBalls(const Rectangle& paddleRect) {
        for (auto& ball : balls) {
            if (ball.stuckToPaddle && ball.active) {
                // Mantener la pelota pegada al paddle
                ball.rect.x = paddleRect.x + ball.paddleOffset;
                ball.rect.y = paddleRect.y - ball.rect.height;
                
                // Asegurar que la pelota no se salga de los límites de la pantalla
                if (ball.rect.x < 0) {
                    ball.rect.x = 0;
                    ball.paddleOffset = ball.rect.x - paddleRect.x;
                }
                if (ball.rect.x + ball.rect.width > Consts::WINDOW_WIDTH) {
                    ball.rect.x = Consts::WINDOW_WIDTH - ball.rect.width;
                    ball.paddleOffset = ball.rect.x - paddleRect.x;
                }
            }
        }
    }
    
    void launchStuckBalls() {
        for (auto& ball : balls) {
            if (ball.stuckToPaddle && ball.active) {
                ball.stuckToPaddle = false;
                // Usar la dirección prevista
                ball.vx = ball.previewVx;
                ball.vy = ball.previewVy;
            }
        }
    }
    
    bool hasStuckBalls() {
        for (const auto& ball : balls) {
            if (ball.stuckToPaddle && ball.active) {
                return true;
            }
        }
        return false;
    }
    
    void rebound(const Rectangle& otherRect, Ball& ball) {
        // Calcular centros
        float ballCenterX = ball.rect.x + (ball.rect.width / 2.0f);
        float ballCenterY = ball.rect.y + (ball.rect.height / 2.0f);
        float otherCenterX = otherRect.x + (otherRect.width / 2.0f);
        float otherCenterY = otherRect.y + (otherRect.height / 2.0f);
        
        // Calcular solapamientos en cada eje
        float overlapX = std::min(
            ball.rect.x + ball.rect.width - otherRect.x,
            otherRect.x + otherRect.width - ball.rect.x
        );
        
        float overlapY = std::min(
            ball.rect.y + ball.rect.height - otherRect.y,
            otherRect.y + otherRect.height - ball.rect.y
        );
        
        if (overlapX < overlapY) {
            if (ballCenterX < otherCenterX) {
                ball.rect.x = otherRect.x - ball.rect.width - 1.0f;
                setDirectionLeft(ball);
            } else {
                ball.rect.x = otherRect.x + otherRect.width + 1.0f;
                setDirectionRight(ball);
            }
        } else {
            if (ballCenterY < otherCenterY) {
                ball.rect.y = otherRect.y - ball.rect.height - 1.0f;
                setDirectionUp(ball);
            } else {
                ball.rect.y = otherRect.y + otherRect.height + 1.0f;
                setDirectionDown(ball);
            }
        }
    }

    void reverseY(Ball& ball) {
        ball.vy = -ball.vy;
    }
    
    void reverseX(Ball& ball) {
        ball.vx = -ball.vx;
    }
    
    void setDirectionUp(Ball& ball) {
        ball.vy = -Consts::BALL_SPEED;
    }
    
    void setDirectionDown(Ball& ball) {
        ball.vy = Consts::BALL_SPEED;
    }
    
    void setDirectionLeft(Ball& ball) {
        ball.vx = -Consts::BALL_SPEED;
    }
    
    void setDirectionRight(Ball& ball) {
        ball.vx = Consts::BALL_SPEED;
    }
    
    bool isOut() {
        return balls.empty();
    }
    
    void show() {
        if (!texturesLoaded) {
            loadTextures();
        }
        
        for (const auto& ball : balls) {
            if (ball.active) {
                // Mostrar línea de dirección ANTES de la pelota para que aparezca detrás
                if (ball.stuckToPaddle) {
                    showDirectionLine(ball);
                }
                
                Utils::DrawTextureScaled(ballTexture, ball.rect.x, ball.rect.y, (float)Consts::BALL_SIZE, (float)Consts::BALL_SIZE);
            }
        }
    }
    
    void showDirectionLine(const Ball& ball) {
        // Calcular el centro de la pelota
        float ballCenterX = ball.rect.x + ball.rect.width / 2;
        float ballCenterY = ball.rect.y + ball.rect.height / 2;
        
        // Usar la dirección prevista almacenada
        float directionX = ball.previewVx;
        float directionY = ball.previewVy;
        
        // Normalizar la dirección para la línea
        float length = sqrt(directionX * directionX + directionY * directionY);
        float normalizedX = directionX / length;
        float normalizedY = directionY / length;
        
        // Longitud total de la línea de dirección
        float lineLength = 160.0f;
        int numBalls = 40; // Número de pelotitas a lo largo de la trayectoria
        float ballSize = 4.0f; // Tamaño de cada pelotita
        float spacing = lineLength / numBalls; // Espaciado entre pelotitas (muy pegadas)
        
        // Dibujar pelotitas rojas con transparencia decreciente
        for (int i = 0; i < numBalls; i++) {
            float t = (float)i / (numBalls - 1); // Normalizado de 0 a 1
            
            // Calcular posición de la pelotita
            float x = ballCenterX + normalizedX * spacing * i;
            float y = ballCenterY + normalizedY * spacing * i;
            
            // Calcular transparencia decreciente (de 220 a 20)
            unsigned char alpha = (unsigned char)(220 * (1.0f - t * t));
            
            // Color rojo con transparencia variable
            Color ballColor = {220, 60, 60, alpha};
            
            // Dibujar pelotita como círculo sólido
            DrawCircle((int)x, (int)y, ballSize, ballColor);
        }
    }
    
    void reset() {
        balls.clear();
        addBall();
    }
    
    void reset(const Rectangle& paddleRect) {
        balls.clear();
        addBallOnPaddle(paddleRect);
    }
    
    void addBallOnPaddle(const Rectangle& paddleRect) {
        Ball newBall;
        newBall.rect.width = newBall.rect.height = Consts::BALL_SIZE;
        newBall.vx = 0;
        newBall.vy = 0;
        newBall.active = true;
        newBall.stuckToPaddle = true;
        centerOnPaddle(newBall, paddleRect);
        // Calcular offset relativo al paddle
        newBall.paddleOffset = newBall.rect.x - paddleRect.x;
        // Calcular dirección prevista (fija hasta el lanzamiento)
        newBall.previewVx = (std::rand() % 2 ? 1 : -1) * Consts::BALL_SPEED;
        newBall.previewVy = -Consts::BALL_SPEED;
        balls.push_back(newBall);
    }
    
    std::vector<Ball>& getBalls() {
        return balls;
    }

private:
    std::vector<Ball> balls;
    bool texturesLoaded;
    Texture2D ballTexture;
};
