#pragma once
#include <raylib.h>
#include "../functions/Consts.hpp"
#include "../enum/ItemType.hpp"

class Item {
public:
    Item(float x, float y, ItemType type) : type(type), active(true) {
        rect.x = x;
        rect.y = y;
        rect.width = Consts::ITEM_SIZE;
        rect.height = Consts::ITEM_SIZE;
        vy = Consts::ITEM_FALL_SPEED;
    }
    
    void update() {
        if (!active) return;
        
        rect.y += vy;
        
        // Si el item sale de la pantalla por abajo, se desactiva
        if (rect.y > Consts::WINDOW_HEIGHT) {
            active = false;
        }
    }
    
    void show() {
        if (!active) return;
        
        Color color;
        switch (type) {
            case ItemType::EXPAND_PADDLE:
                color = GREEN;
                break;
            case ItemType::DUPLICATE_BALL:
                color = YELLOW;
                break;
            default:
                color = WHITE;
                break;
        }
        
        DrawRectangleRec(rect, color);
        
        // Dibujar un símbolo en el item para identificarlo
        float centerX = rect.x + rect.width / 2;
        float centerY = rect.y + rect.height / 2;
        
        switch (type) {
            case ItemType::EXPAND_PADDLE:
                // Dibujar líneas horizontales para indicar expansión
                DrawLine(centerX - 6, centerY, centerX + 6, centerY, BLACK);
                DrawLine(centerX - 4, centerY - 2, centerX + 4, centerY - 2, BLACK);
                DrawLine(centerX - 4, centerY + 2, centerX + 4, centerY + 2, BLACK);
                break;
            case ItemType::DUPLICATE_BALL:
                // Dibujar dos círculos pequeños
                DrawCircle(centerX - 3, centerY, 2, BLACK);
                DrawCircle(centerX + 3, centerY, 2, BLACK);
                break;
        }
    }
    
    bool checkCollision(const Rectangle& otherRect) {
        if (!active) return false;
        
        bool collision = CheckCollisionRecs(rect, otherRect);
        if (collision) {
            active = false;
        }
        return collision;
    }
    
    // Getters
    ItemType getType() const { return type; }
    bool isActive() const { return active; }
    Rectangle getRect() const { return rect; }
    
    // Setter
    void setActive(bool state) { active = state; }
    
private:
    Rectangle rect{};
    ItemType type;
    float vy;
    bool active;
};
