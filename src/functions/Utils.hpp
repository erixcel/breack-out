#pragma once
#include <raylib.h>
#include <string>
#include "Consts.hpp"

namespace Utils
{
    // Método para renderizar textura escalada al tamaño especificado
    inline void DrawTextureScaled(Texture2D texture, float x, float y, float width, float height, Color tint = WHITE)
    {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
        Rectangle dest = { x, y, width, height };
        Vector2 origin = { 0, 0 };
        
        DrawTexturePro(texture, source, dest, origin, 0.0f, tint);
    }

    inline void DrawTextureThreeSlice(Texture2D texture, float x, float y, float width, float height, float borderWidth, Color tint = WHITE)
    {
        float textureWidth = (float)texture.width;
        float textureHeight = (float)texture.height;
        
        if (width < borderWidth * 2) width = borderWidth * 2;
        
        float centerWidth = width - (borderWidth * 2);
        float centerSourceWidth = textureWidth - (borderWidth * 2);
        
        Rectangle sourceLeft = { 0, 0, borderWidth, textureHeight };
        Rectangle destLeft = { x, y, borderWidth, height };
        DrawTexturePro(texture, sourceLeft, destLeft, {0, 0}, 0.0f, tint);
        
        Rectangle sourceCenter = { borderWidth, 0, centerSourceWidth, textureHeight };
        Rectangle destCenter = { x + borderWidth, y, centerWidth, height };
        DrawTexturePro(texture, sourceCenter, destCenter, {0, 0}, 0.0f, tint);
        
        Rectangle sourceRight = { textureWidth - borderWidth, 0, borderWidth, textureHeight };
        Rectangle destRight = { x + width - borderWidth, y, borderWidth, height };
        DrawTexturePro(texture, sourceRight, destRight, {0, 0}, 0.0f, tint);
    }
    
    inline void showModal(std::string title, std::string message)
    {
        // Configuración del modal
        int modalWidth = 400;
        int modalHeight = 200;
        int modalX = (Consts::WINDOW_WIDTH - modalWidth) / 2;
        int modalY = (Consts::WINDOW_HEIGHT - modalHeight) / 2;
        
        // Fondo semi-transparente
        DrawRectangle(0, 0, Consts::WINDOW_WIDTH, Consts::WINDOW_HEIGHT, Fade(BLACK, 0.5f));
        
        // Fondo del modal
        DrawRectangle(modalX, modalY, modalWidth, modalHeight, WHITE);
        DrawRectangleLines(modalX, modalY, modalWidth, modalHeight, BLACK);
        
        // Título
        int titleFontSize = 24;
        Vector2 titleSize = MeasureTextEx(GetFontDefault(), title.c_str(), titleFontSize, 1);
        int titleX = modalX + (modalWidth - titleSize.x) / 2;
        int titleY = modalY + 30;
        DrawText(title.c_str(), titleX, titleY, titleFontSize, BLACK);
        
        // Mensaje
        int messageFontSize = 18;
        Vector2 messageSize = MeasureTextEx(GetFontDefault(), message.c_str(), messageFontSize, 1);
        int messageX = modalX + (modalWidth - messageSize.x) / 2;
        int messageY = titleY + 50;
        DrawText(message.c_str(), messageX, messageY, messageFontSize, DARKGRAY);
        
        // Botón "Continuar"
        int buttonWidth = 120;
        int buttonHeight = 40;
        int buttonX = modalX + (modalWidth - buttonWidth) / 2;
        int buttonY = modalY + modalHeight - 60;
        
        DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, LIGHTGRAY);
        DrawRectangleLines(buttonX, buttonY, buttonWidth, buttonHeight, BLACK);
        
        const char* buttonText = "Continuar";
        int buttonFontSize = 16;
        Vector2 buttonTextSize = MeasureTextEx(GetFontDefault(), buttonText, buttonFontSize, 1);
        int buttonTextX = buttonX + (buttonWidth - buttonTextSize.x) / 2;
        int buttonTextY = buttonY + (buttonHeight - buttonTextSize.y) / 2;
        DrawText(buttonText, buttonTextX, buttonTextY, buttonFontSize, BLACK);
    }
    
    inline void showVictoryModal()
    {
        showModal("¡VICTORIA!", "Has destruido todos los bloques.");
    }

    inline void showDefeatModal()
    {
        showModal("DERROTA", "La bola cayó fuera. Intenta de nuevo.");
    }
}