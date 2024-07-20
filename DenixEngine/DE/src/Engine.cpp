#include "Engine.h"

#include <SDL3/SDL.h>

namespace Denix
{
    Engine* Engine::s_EngineInstance{nullptr};
    
    Engine::Engine()
    {
        s_EngineInstance = this;
    }

    Engine::~Engine()
    {
        s_EngineInstance = nullptr;
    }

    void Engine::Run()
    {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result < 0) {
            SDL_Log("SDL_Init error: %s", SDL_GetError());
        }

        window = SDL_CreateWindow("sdl3 hello world", 800, 600, 0);
        if (window == NULL) {
            SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        }
    
        renderer = SDL_CreateRenderer(window, NULL);
        if (renderer == NULL) {
            SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        }

        SDL_Log("SDL3 initialized");

        SDL_Event event;
        int quit = 0;
        while(!quit)
        {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_EVENT_QUIT:
                    SDL_Log("SDL3 event quit");
                    quit = 1;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        SDL_Log("SDL3 ESC key quit");
                        quit = 1;
                    }
                }
            }
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
            SDL_RenderClear(renderer);

            SDL_RenderPresent(renderer);
            SDL_Delay(1);
        }
        
        SDL_Log("SDL3 shutdown");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }
}
