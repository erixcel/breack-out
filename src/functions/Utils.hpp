#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Consts.hpp"

namespace Utils
{
    inline bool checkCollision(const SDL_Rect &a, const SDL_Rect &b)
    {
        return (a.x < b.x + b.w &&
                a.x + a.w > b.x &&
                a.y < b.y + b.h &&
                a.y + a.h > b.y);
    }

    inline void showModal(SDL_Renderer *r,
                          const std::string &title,
                          const std::string &message)
    {
        std::string js =
            "alert('" + title + "\\n\\n" + message + "');";
        emscripten_run_script(js.c_str());
    }
    
    inline void showVictoryModal(SDL_Renderer *r)
    {
        showModal(r, "🏆 ¡VICTORIA! 🏆", "✨ Has destruido todos los bloques. 🎉");
    }

    inline void showDefeatModal(SDL_Renderer *r)
    {
        showModal(r, "😢 DERROTA 😢", "💔 La bola cayó fuera. Intenta de nuevo. 🔄");
    }
}