#include <SDL2/SDL.h>
#include <emscripten.h>

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;

void loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
        if (e.type == SDL_QUIT) { 
            SDL_Quit(); 
            return; 
        }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window   = SDL_CreateWindow("basic_01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
