#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "sdl_util.h"

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    basicSetup(800, 800, (SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_TIMER),
               "your nan", &window, &renderer);


    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    char running = 1;

    while(running)
    {
        SDL_RenderClear(renderer);
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    running = 0;
                    break;
                default:
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(60 / 1000);
    }


    safeQuit(window, renderer);
    return 0;
}
