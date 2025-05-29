#pragma once
#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"

class Ball
{
public:
    Ball()
    {
        rect.width = rect.height = Consts::BALL_SIZE;
    }
    void center()
    {
        rect.x = (Consts::WINDOW_WIDTH - rect.width) / 2;
        rect.y = (Consts::WINDOW_HEIGHT - rect.height) / 2;
    }
    void startMovement()
    {
        vx = (std::rand() % 2 ? 1 : -1) * Consts::BALL_SPEED;
        vy = -Consts::BALL_SPEED;
    }
    void update()
    {
        rect.x += vx;
        rect.y += vy;

        if (rect.x <= 0 || rect.x + rect.width >= Consts::WINDOW_WIDTH)
            reverseX();
        if (rect.y <= 0)
            reverseY();
    }
    
    void rebound(const Rectangle& otherRect) {

        // Calcular centros
        float ballCenterX = rect.x + (rect.width / 2.0f);
        float ballCenterY = rect.y + (rect.height / 2.0f);
        float otherCenterX = otherRect.x + (otherRect.width / 2.0f);
        float otherCenterY = otherRect.y + (otherRect.height / 2.0f);
        
        // Calcular solapamientos en cada eje
        float overlapX = std::min(
            rect.x + rect.width - otherRect.x,
            otherRect.x + otherRect.width - rect.x
        );
        
        float overlapY = std::min(
            rect.y + rect.height - otherRect.y,
            otherRect.y + otherRect.height - rect.y
        );
        
        if (overlapX < overlapY) {
            if (ballCenterX < otherCenterX) {
                rect.x = otherRect.x - rect.width - 1.0f;
                setDirectionLeft();
            } else {
                rect.x = otherRect.x + otherRect.width + 1.0f;
                setDirectionRight();
            }
        } else {
            if (ballCenterY < otherCenterY) {
                rect.y = otherRect.y - rect.height - 1.0f;
                setDirectionUp();
            } else {
                rect.y = otherRect.y + otherRect.height + 1.0f;
                setDirectionDown();
            }
        }
    }

    void reverseY()
    {
        vy = -vy;
    }
    void reverseX()
    {
        vx = -vx;
    }
    void setDirectionUp()
    {
        vy = -Consts::BALL_SPEED;

    }
    void setDirectionDown()
    {
        vy = Consts::BALL_SPEED;
    }
    void setDirectionLeft()
    {
        vx = -Consts::BALL_SPEED;
    }
    void setDirectionRight()
    {
        vx = Consts::BALL_SPEED;
    }
    bool isOut() 
    {
        return rect.y + rect.height >= Consts::WINDOW_HEIGHT;
    }
    void show()
    {
        DrawRectangleRec(rect, WHITE);
    }
    void reset() 
    {
        center();
        startMovement();
    }
    Rectangle &getRect()
    {
        return rect;
    }

private:
    Rectangle rect{};
    float vx = 0, vy = 0;
};
