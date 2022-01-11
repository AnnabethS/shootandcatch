#include "bullet.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define SPEED 0.05
#define PI 3.141529

void initBullet(bullet_t* bullet, float rotation,
                SDL_Texture* texture, float startX, float startY)
{
    bullet->rect.x = startX;
    bullet->rect.y = startY;
    int w,h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    w /= 4;
    h /= 4;
    bullet->rect.w = (float) w;
    bullet->rect.h = (float) h;

    bullet->texture = texture;
    bullet->textureCentre.x = (float)w/2;
    bullet->textureCentre.y = (float)h/2;

    bullet->speed = SPEED;
    bullet->rotation = rotation - 90;

    bullet->perFrameMove.x = cos(bullet->rotation * (PI / 180)) * bullet->speed;
    bullet->perFrameMove.y = sin(bullet->rotation * (PI / 180)) * bullet->speed;

    bullet->rotation = rotation;
    
    bullet->next = NULL;
}

void moveBullet(bullet_t* bullet)
{
    bullet->rect.x += bullet->perFrameMove.x;
    bullet->rect.y += bullet->perFrameMove.y;
}

int updateBullet(bullet_t* bullet, int screenWidth, int screenHeight)
{
    bullet->rect.x += bullet->perFrameMove.x;
    bullet->rect.y += bullet->perFrameMove.y;
    if(bullet->rect.x < 0 | bullet->rect.x + bullet->rect.w >= screenWidth |
	    bullet->rect.y < 0 | bullet->rect.y + bullet->rect.h >= screenHeight)
	    return 1;
    else
	    return 0;
}

void drawBullet(bullet_t* bullet, SDL_Renderer* renderer)
{
    SDL_RenderCopyExF(renderer, bullet->texture, NULL, &bullet->rect,
                      bullet->rotation, &bullet->textureCentre, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRectF(renderer, &(bullet->rect));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
