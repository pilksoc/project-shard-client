#include <iostream>
#include <iterator>
#include <SDL.h>
#include <testing_h/logger.h>
#include "./WebsocketClient.h"

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

typedef struct state_t {
    WebsocketClient *wsClient;
    SDL_Window *window;
    SDL_Renderer *renderer;
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
        lprintf(LOG_INFO, "There are %lu items in the recv queue\n", resp->messages.size());
        delete resp;
    }

    SDL_Rect squareRect;
    squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

    squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
    squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(state->renderer);
    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(state->renderer, &squareRect);
    SDL_RenderPresent(state->renderer);
    return 0;
}

int main(int argc, char **argv)
{
    std::cerr << "Version: " << PSC_PROJECT_VERSION << std::endl;
    std::cerr << "OS: " << PSC_OS << std::endl;
    std::cerr << PSC_PROJECT_NAME << " - " PSC_PROJECT_VERSION << std::endl;

    state_t state = {};
    memset(&state, 0, sizeof(state));
    state.wsClient = new WebsocketClient("http://project-shard.leonic.moe/ws");

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    state.window = SDL_CreateWindow("Basic C++ SDL project",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);
    if(!state.window) {
        std::cout << "Window could not be created!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
        // Create renderer
        state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
        if(!state.renderer) {
            std::cout << "Renderer could not be created!" << std::endl
                      << "SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }


        // Event loop exit flag
        bool quit = false;

        // Event loop
        while(!quit) {
            quit = poll(&state);
        }

        SDL_DestroyRenderer(state.renderer);
    }

    SDL_DestroyWindow(state.window);
    SDL_Quit();

    if (state.wsClient != nullptr) {
        delete state.wsClient;
    }

    return 0;
}
