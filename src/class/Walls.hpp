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
                    break; // Solo rebota, no se rompe
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
        
        // Configuración de la grilla para las paredes usando parámetros
        const int gridCols = cols;
        const int gridRows = rows;
        const int startY = Consts::WALL_START_Y;
        
        // Calcular el ancho total y centrar (igual que Blocks)
        int totalW = gridCols * Consts::WALL_SIZE + (gridCols - 1) * Consts::WALL_GAP;
        int offsetX = (Consts::WINDOW_WIDTH - totalW) / 2;
        
        const int centerCol = gridCols / 2;  // Columna central para simetría
        
        // Generar paredes aleatoriamente de forma simétrica
        for (int row = 0; row < gridRows; ++row) {
            for (int col = 0; col <= centerCol; ++col) {
                float random_value = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                
                // Solo generar pared si el valor aleatorio es menor que la probabilidad
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
        
        // Asegurar que haya al menos algunas paredes simétricas si la generación aleatoria no creó ninguna
        if (walls.empty()) {
            // Crear algunas paredes de respaldo simétricas usando la misma lógica
            Wall wall1;
            wall1.rect = {
                (float)(offsetX + 5 * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                (float)Consts::WALL_START_Y,
                (float)Consts::WALL_SIZE,
                (float)Consts::WALL_SIZE
            };
            walls.push_back(wall1);
            
            Wall wall2;
            wall2.rect = {
                (float)(offsetX + (gridCols - 6) * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                (float)Consts::WALL_START_Y,
                (float)Consts::WALL_SIZE,
                (float)Consts::WALL_SIZE
            };
            walls.push_back(wall2);
            
            if (gridRows > 1) {
                Wall wall3;
                wall3.rect = {
                    (float)(offsetX + 8 * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                    (float)(Consts::WALL_START_Y + Consts::WALL_SIZE + Consts::WALL_GAP),
                    (float)Consts::WALL_SIZE,
                    (float)Consts::WALL_SIZE
                };
                walls.push_back(wall3);
                
                Wall wall4;
                wall4.rect = {
                    (float)(offsetX + (gridCols - 9) * (Consts::WALL_SIZE + Consts::WALL_GAP)),
                    (float)(Consts::WALL_START_Y + Consts::WALL_SIZE + Consts::WALL_GAP),
                    (float)Consts::WALL_SIZE,
                    (float)Consts::WALL_SIZE
                };
                walls.push_back(wall4);
            }
        }
    }
};
