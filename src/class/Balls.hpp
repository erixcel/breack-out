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
    };

    Balls() {
        // Initialize with one ball by default
        addBall();
    }
    
    void addBall() {
        Ball newBall;
        newBall.rect.width = newBall.rect.height = Consts::BALL_SIZE;
        newBall.vx = 0;
        newBall.vy = 0;
        newBall.active = true;
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
        balls.push_back(newBall);
    }
    
    void center(Ball& ball) {
        ball.rect.x = (Consts::WINDOW_WIDTH - ball.rect.width) / 2;
        ball.rect.y = (Consts::WINDOW_HEIGHT - ball.rect.height) / 2;
    }
    
    void startMovement(Ball& ball) {
        ball.vx = (std::rand() % 2 ? 1 : -1) * Consts::BALL_SPEED;
        ball.vy = -Consts::BALL_SPEED;
    }
    
    void update() {
        for (auto& ball : balls) {
            if (!ball.active) continue;
            
            ball.rect.x += ball.vx;
            ball.rect.y += ball.vy;

            if (ball.rect.x <= 0 || ball.rect.x + ball.rect.width >= Consts::WINDOW_WIDTH)
                reverseX(ball);
            if (ball.rect.y <= 0)
                reverseY(ball);
                
            // Check if ball is out of bounds
            if (ball.rect.y + ball.rect.height >= Consts::WINDOW_HEIGHT) {
                ball.active = false;
            }
        }
        
        // Remove inactive balls
        balls.erase(std::remove_if(balls.begin(), balls.end(), 
            [](const Ball& ball) { return !ball.active; }), balls.end());
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
        for (const auto& ball : balls) {
            if (ball.active) {
                DrawRectangleRec(ball.rect, WHITE);
            }
        }
    }
    
    void reset() {
        balls.clear();
        addBall();
    }
    
    std::vector<Ball>& getBalls() {
        return balls;
    }

private:
    std::vector<Ball> balls;
};
