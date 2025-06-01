#include <iostream>
#include <iterator>
#include <SDL.h>
#include "./WebsocketClient.h"

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

typedef struct state_t {
    WebsocketClient *wsClient;
    SDL_Window *window;
} state_t;

static int poll(state_t *state)
{
    SDL_Event e;
    SDL_WaitEvent(&e);

    if(e.type == SDL_QUIT) {
        return 1;
    }

    if (state->wsClient != nullptr) {
        auto resp = state->wsClient->Poll();
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &squareRect);
    SDL_RenderPresent(renderer);
    return 0;
}

int main(int argc, char **argv)
{
    std::cerr << "Version: " << PSC_PROJECT_VERSION << std::endl;
    std::cerr << "OS: " << PSC_OS << std::endl;
    std::cerr << PSC_PROJECT_NAME << " - " PSC_PROJECT_VERSION << std::endl;

    state_t state = {};
    state.wsClient = new WebsocketClient("http://project-shard.leonic.moe/ws");

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }

    // Create window
    state.window = SDL_CreateWindow("Basic C++ SDL project",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);
    if(!window) {
        std::cout << "Window could not be created!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
        // Create renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer) {
            std::cout << "Renderer could not be created!" << std::endl
                      << "SDL_Error: " << SDL_GetError() << std::endl;
        }

        SDL_Rect squareRect;

        squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
        squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

        squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
        squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;


        // Event loop exit flag
        bool quit = false;

        // Event loop
        while(!quit) {
            quit = poll(&state);
        }

        SDL_DestroyRenderer(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (state.wsClient != nullptr) {
        delete state.wsClient;
    }

    return 0;
}
