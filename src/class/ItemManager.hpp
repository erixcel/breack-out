#pragma once
#include <vector>
#include <random>
#include <ctime>
#include "Item.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include "../functions/Consts.hpp"

class ItemManager {
public:
    ItemManager() {
        // Inicializar el generador de números aleatorios
        std::srand(std::time(nullptr));
    }
    
    // Crear un item en una posición específica (cuando se destruye un bloque)
    void createItem(float x, float y) {
        // Verificar si debe aparecer un item (probabilidad)
        if ((std::rand() % 100) / 100.0f > Consts::ITEM_DROP_CHANCE) {
            return;
        }
        
        // Elegir aleatoriamente el tipo de item
        ItemType type = (std::rand() % 2 == 0) ? ItemType::EXPAND_PADDLE : ItemType::DUPLICATE_BALL;
        
        // Centrar el item en el bloque destruido
        float itemX = x + (Consts::BLOCK_W - Consts::ITEM_SIZE) / 2;
        float itemY = y + (Consts::BLOCK_H - Consts::ITEM_SIZE) / 2;
        
        items.emplace_back(itemX, itemY, type);
    }
    
    void update() {
        for (auto& item : items) {
            item.update();
        }
        
        // Eliminar items inactivos
        items.erase(
            std::remove_if(items.begin(), items.end(),
                [](const Item& item) { return !item.isActive(); }),
            items.end()
        );
    }
    
    void show() {
        for (auto& item : items) {
            item.show();
        }
    }
    
    // Verificar colisiones con la paleta y aplicar efectos
    void checkPaddleCollisions(Paddle* paddle, std::vector<Ball>* balls) {
        for (auto& item : items) {
            if (item.checkCollision(paddle->getRect())) {
                applyItemEffect(item.getType(), paddle, balls);
            }
        }
    }
    
    void reset() {
        items.clear();
    }
    
    size_t getItemCount() const {
        return items.size();
    }
    
private:
    std::vector<Item> items;
    
    void applyItemEffect(ItemType type, Paddle* paddle, std::vector<Ball>* balls) {
        switch (type) {
            case ItemType::EXPAND_PADDLE:
                paddle->expand();
                break;
            case ItemType::DUPLICATE_BALL:
                duplicateBalls(balls);
                break;
        }
    }
    
    void duplicateBalls(std::vector<Ball>* balls) {
        size_t originalSize = balls->size();
        for (size_t i = 0; i < originalSize; ++i) {
            Ball newBall = (*balls)[i];
            // Cambiar ligeramente la dirección de la nueva pelota
            newBall.reverseX();
            balls->push_back(newBall);
        }
    }
};
