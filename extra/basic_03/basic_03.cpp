#include <SDL2/SDL.h>
#include <emscripten.h>
#include <vector>

const int W = 800, H = 600;          // ventana
const int ROWS = 5, COLS = 10;       // bloques
const int BW = 60, BH = 20, GAP = 4; // ancho, alto, separación

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;
std::vector<SDL_Rect>  blocks;

void createBlocks() {
    blocks.clear();
    int totalW  = COLS * BW + (COLS - 1) * GAP;
    int offsetX = (W - totalW) / 2;
    int offsetY = 60;                               // margen superior
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c) {
            SDL_Rect b{ offsetX + c * (BW + GAP),
                         offsetY + r * (BH + GAP),
                         BW, BH };
            blocks.push_back(b);
        }
}

void loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
        if (e.type == SDL_QUIT) { SDL_Quit(); return; }

    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // morado
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255); // bloques
    for (const auto& b : blocks) SDL_RenderFillRect(renderer, &b);

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window   = SDL_CreateWindow("basic_03", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    createBlocks();
    emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
