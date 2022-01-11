#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include "sdl_util.h"
#include "player.h"
#include "bullet.h"

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900

int main()
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    basicSetup(SCREENWIDTH, SCREENHEIGHT,
               (SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_TIMER),
               "your nan", &window, &renderer);

    SDL_Surface* pSurface = IMG_Load("res/triangle.png");
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(renderer, pSurface);
    SDL_FreeSurface(pSurface);

    player_t* player = initPlayer(50, 50, 0, pTexture, 0.15);
    player->speed = 0.03;

    bullet_t* bulletListHead = NULL;
    bullet_t* bulletListTail = NULL;

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    char running = 1;

    int playerMoveX = 0;
    int playerMoveY = 0;

    int mouseX = 0;
    int mouseY = 0;

    while(running)
    {
        //update
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
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    playerMoveY = -1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    playerMoveY = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    playerMoveX = -1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    playerMoveX = 1;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if(playerMoveY == -1)
                        playerMoveY = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(playerMoveY == 1)
                        playerMoveY = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    if(playerMoveX == -1)
                        playerMoveX = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    if(playerMoveX == 1)
                        playerMoveX = 0;
                    break;
                default:
                    break;
                }
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(bulletListHead == NULL)
                {
                    bulletListHead = malloc(sizeof(bullet_t));
                    bulletListTail = bulletListHead;
                    initBullet(bulletListHead, player->rotation,
                              pTexture, player->rect.x, player->rect.y);
                }
                else
                {
                    bulletListTail->next = malloc(sizeof(bullet_t));
                    bulletListTail = bulletListTail->next;
                    initBullet(bulletListTail, player->rotation,
                               pTexture, player->rect.x, player->rect.y);
                }
                break;
            }
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        
        movePlayer(player, playerMoveX, playerMoveY);
        rotatePlayer(player, mouseX, mouseY);

        bullet_t* prevBullet_ptr = NULL;
        bullet_t* bullet_ptr = bulletListHead;
        
        
        while(bullet_ptr != NULL)
        {
	        int bulletExited = updateBullet(bullet_ptr, SCREENWIDTH, SCREENHEIGHT);
	        if(bulletExited)
	        {
		        if(prevBullet_ptr != NULL)
		        {
					prevBullet_ptr->next = bullet_ptr->next;
					free(bullet_ptr);
					bullet_ptr = prevBullet_ptr->next;
		        }
		        else
		        {
			        if(bulletListHead == bulletListTail)
			        {
				        bulletListTail = NULL;
				        free(bulletListTail);
				        bulletListHead = NULL;
			        }
			        else
			        {
				        bullet_t* newHead = bulletListHead->next;
						free(bulletListHead);
						bulletListHead = newHead;
			        }
		        }
	        }
            prevBullet_ptr = bullet_ptr;
            bullet_ptr = bullet_ptr->next;
        }
        
        // draw
        SDL_RenderClear(renderer);
            
        drawPlayer(renderer, player);

        bullet_ptr = bulletListHead;
        
        while(bullet_ptr != NULL)
        {
            drawBullet(bullet_ptr, renderer);
            bullet_ptr = bullet_ptr->next;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(60 / 1000);
    }


    safeQuit(window, renderer);
    return 0;
}
