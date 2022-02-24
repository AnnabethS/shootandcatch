#include "bullet.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include "textures.h"
#include "anna-layer.h"

#define SPEED 10
#define PI 3.14159265
#define RAD2DEG 180 / PI
#define DEG2RAD PI / 180

void initBullet(bullet_t* bullet, float rotation,
                SDL_Texture* texture, float startX, float startY)
{
    int w,h;
    SDL_QueryTexture(textures.bullet, NULL, NULL, &w, &h);
    w *= 4;
    h *= 4;
    bullet->rect.w = (float) w;
    bullet->rect.h = (float) h;

    bullet->textureCentre.x = (float)w/2;
    bullet->textureCentre.y = (float)h/2;

    bullet->rect.x = startX - bullet->rect.w/2;
    bullet->rect.y = startY - bullet->rect.h/2;

    bullet->speed = SPEED;
    bullet->rotation = rotation - 90;

    bullet->perFrameMove.x = cos(bullet->rotation * DEG2RAD) * bullet->speed;
    bullet->perFrameMove.y = sin(bullet->rotation * DEG2RAD) * bullet->speed;

    bullet->currentBounces = 4;
    
    bullet->corners[0].x = 0;
    bullet->corners[0].y = 0;
    bullet->corners[1].x = bullet->rect.w;
    bullet->corners[1].y = 0;
    bullet->corners[2].x = bullet->rect.w;
    bullet->corners[2].y = bullet->rect.h;
    bullet->corners[3].x = 0;
    bullet->corners[3].y = bullet->rect.h;

    rotateBulletCorners(bullet, (bullet->rotation));
    
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

	bullet->currentBounces -= bounced;

	if(bullet->currentBounces == 0)
	{
		return 1;
	}

	bullet->rect.x += bullet->perFrameMove.x;
	bullet->rect.y += bullet->perFrameMove.y;

	if(bounced)
	{
		bullet->rotation = (atan2(bullet->perFrameMove.y, bullet->perFrameMove.x)
							* RAD2DEG);
		rotateBulletCorners(bullet, bullet->rotation);
	}

	return 0;
}

void drawBullet(bullet_t* bullet, SDL_Renderer* renderer)
{
    SDL_RenderCopyExF(renderer, textures.bullet, NULL, &bullet->rect,
                      bullet->rotation, &bullet->textureCentre, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for(int i=0; i<3; i++)
    {
	    SDL_RenderDrawLineF(renderer,
	                        bullet->corners[i].x + bullet->rect.x,
	                        bullet->corners[i].y + bullet->rect.y,
	                        bullet->corners[i+1].x + bullet->rect.x,
	                        bullet->corners[i+1].y + bullet->rect.y);
    }
    SDL_RenderDrawLineF(renderer,
                        bullet->corners[3].x + bullet->rect.x,
                        bullet->corners[3].y + bullet->rect.y,
                        bullet->corners[0].x + bullet->rect.x,
                        bullet->corners[0].y + bullet->rect.y);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void rotateBulletCorners(bullet_t *bullet, float rotation)
{
	if(rotation < 0)
		rotation += 360;

	rotation *= DEG2RAD;
	float w = bullet->rect.w;
	float h = bullet->rect.h;
	float c = cos(rotation);
	float s = sin(rotation);

	bullet->corners[0].x = 0;
	bullet->corners[0].y = 0;

	bullet->corners[1].x = w*c;
	bullet->corners[1].y = w*s;

	bullet->corners[2].x = w*c - h*s;
	bullet->corners[2].y = w*s + h*c;

	bullet->corners[3].x = 0 - h*s;
	bullet->corners[3].y = h*c;

	SDL_FPoint oCentre;
	oCentre.x = bullet->rect.w/2;
	oCentre.y = bullet->rect.h/2;

	SDL_FPoint rCentre;
	rCentre.x = bullet->corners[2].x/2;
	rCentre.y = bullet->corners[2].y/2;

	for(int i=0; i < 4; i++)
	{
		bullet->corners[i].x += oCentre.x - rCentre.x;
		bullet->corners[i].y += oCentre.y - rCentre.y;
	}
}

