#include "bullet.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define SPEED 10
#define PI 3.141529
#define RAD2DEG 180 / PI
#define DEG2RAD PI / 180

void initBullet(bullet_t* bullet, float rotation,
                SDL_Texture* texture, float startX, float startY)
{
    int w,h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    w *= 4;
    h *= 4;
    bullet->rect.w = (float) w;
    bullet->rect.h = (float) h;

    bullet->texture = texture;
    bullet->textureCentre.x = (float)w/2;
    bullet->textureCentre.y = (float)h/2;

    bullet->rect.x = startX - bullet->rect.w/2;
    bullet->rect.y = startY - bullet->rect.h/2;

    bullet->speed = SPEED;
    bullet->rotation = rotation - 90;

    bullet->perFrameMove.x = cos(bullet->rotation * DEG2RAD) * bullet->speed;
    bullet->perFrameMove.y = sin(bullet->rotation * DEG2RAD) * bullet->speed;

    bullet->rotation = rotation;

    bullet->currentBounces = 4;
    
    bullet->next = NULL;
}

void moveBullet(bullet_t* bullet)
{
    bullet->rect.x += bullet->perFrameMove.x;
    bullet->rect.y += bullet->perFrameMove.y;
}

int updateBullet(bullet_t* bullet, int screenWidth, int screenHeight)
{
	int bounced = 0;
    if ((bullet->rect.x + bullet->perFrameMove.x <= 0) ||
        (bullet->rect.x + bullet->perFrameMove.x + bullet->rect.w >= screenWidth))
	{
		bounced = 1;
		bullet->perFrameMove.x *= -1;
	}

	if ((bullet->rect.y + bullet->perFrameMove.y <= 0) ||
	    (bullet->rect.y + bullet->perFrameMove.y + bullet->rect.h >= screenHeight))
	{
		bounced = 1;
		bullet->perFrameMove.y *= -1;
	}

	if(bullet->currentBounces == 0)
	{
		return 1;
	}

	bullet->currentBounces -= bounced;

	bullet->rect.x += bullet->perFrameMove.x;
	bullet->rect.y += bullet->perFrameMove.y;

	bullet->rotation = (atan2(bullet->perFrameMove.y, bullet->perFrameMove.x)
	                    * RAD2DEG);

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
