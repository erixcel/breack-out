#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>
#include <ctime>

const int W = 800, H = 600;

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;

struct Ball { float x, y, vx, vy; int s; } ball;

void resetBall() {
    ball.s  = 12;
    ball.x  = W / 2 - ball.s / 2;
    ball.y  = H / 2 - ball.s / 2;
    ball.vx = (rand() % 2 ? 1 : -1) * 3.0f;
    ball.vy = (rand() % 2 ? 1 : -1) * 3.0f;
}

void loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
        if (e.type == SDL_QUIT) { SDL_Quit(); return; }

    // lógica bolita
    ball.x += ball.vx;  ball.y += ball.vy;
    if (ball.x <= 0 || ball.x + ball.s >= W) ball.vx = -ball.vx;
    if (ball.y <= 0 || ball.y + ball.s >= H) ball.vy = -ball.vy;

    // render
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // morado
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r{ (int)ball.x, (int)ball.y, ball.s, ball.s };
    SDL_RenderFillRect(renderer, &r);

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window   = SDL_CreateWindow("basic_04", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    srand((unsigned)time(nullptr));
    resetBall();
    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
