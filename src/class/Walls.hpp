#pragma once
#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
#include "Balls.hpp"

struct Wall {
    Rectangle rect;
};

class Walls {
public:
    Walls(int rows, int cols) : rows(rows), cols(cols), texturesLoaded(false) {
        build();
    }
    
    ~Walls() {
        if (texturesLoaded) {
            UnloadTexture(wallTexture);
        }
    }
    
    void loadTextures() {
        if (!texturesLoaded) {
            wallTexture = LoadTexture("sprites/wall.png");
            texturesLoaded = true;
        }
    }

    void checkCollisions(Balls* balls) {
        for (auto& ball : balls->getBalls()) {
            if (!ball.active) continue;
            
            for (const auto& wall : walls) {
                if (CheckCollisionRecs(ball.rect, wall.rect)) {
                    balls->rebound(wall.rect, ball);
                    break;
                }
            }
        }
    }
    
    void show() {
        if (!texturesLoaded) {
            loadTextures();
        }
        
        for (const auto& wall : walls) {
            Utils::DrawTextureScaled(wallTexture, wall.rect.x, wall.rect.y, (float)Consts::WALL_SIZE, (float)Consts::WALL_SIZE, WHITE);
        }
    }
    
    void reset() {
        build();
    }

private:
    std::vector<Wall> walls;
    int rows, cols;
    bool texturesLoaded;
    Texture2D wallTexture;
      
    void build() {
        walls.clear();
        
        const int gridCols = cols;
        const int gridRows = rows;
        const int startY = Consts::WALL_START_Y;
        
        int totalW = gridCols * Consts::WALL_SIZE + (gridCols - 1) * Consts::WALL_GAP;
        int offsetX = (Consts::WINDOW_WIDTH - totalW) / 2;
        
        const int centerCol = gridCols / 2;

        for (int row = 0; row < gridRows; ++row) {
            for (int col = 0; col <= centerCol; ++col) {
                float random_value = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                
                if (random_value < Consts::WALL_PROBABILITY) {
                    // Pared izquierda
                    Wall wallLeft;
                    wallLeft.rect = {
                        (float)(offsetX + col * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                        (float)(startY + row * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                        (float)Consts::WALL_SIZE,
                        (float)Consts::WALL_SIZE
                    };
                    walls.push_back(wallLeft);
                    
                    // Pared derecha simétrica (solo si no es la columna central)
                    if (col != centerCol) {
                        int symmetricCol = gridCols - 1 - col;
                        Wall wallRight;
                        wallRight.rect = {
                            (float)(offsetX + symmetricCol * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                            (float)(startY + row * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                            (float)Consts::WALL_SIZE,
                            (float)Consts::WALL_SIZE
                        };
                        walls.push_back(wallRight);
                    }
                }
            }
        }
        
    }
};
