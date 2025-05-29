#include <SDL2/SDL.h>
#include <emscripten.h>

const int W = 800, H = 600;

struct Paddle { float x, y, w, h; } paddle;

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;

void resetPaddle() {
    paddle.w = 100;
    paddle.h = 20;
    paddle.x = (W - paddle.w) / 2;   // aparece centrada
    paddle.y = H - 40;               // 40 px sobre el borde inferior
}

void loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
        if (e.type == SDL_QUIT) { 
            SDL_Quit(); return; 
        }

    // entrada de teclado
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_LEFT]  || keys[SDL_SCANCODE_A]) paddle.x -= 6;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) paddle.x += 6;

    // mantener dentro de los bordes
    if (paddle.x < 0)               paddle.x = 0;
    if (paddle.x + paddle.w > W)    paddle.x = W - paddle.w;

    // render
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // morado
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // barrita blanca
    SDL_Rect r{ (int)paddle.x, (int)paddle.y, (int)paddle.w, (int)paddle.h };
    SDL_RenderFillRect(renderer, &r);

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window   = SDL_CreateWindow("basic_02", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    resetPaddle();
    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
