#include "bullet.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define SPEED 10
#define PI 3.14159265
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
	SDL_FPoint origin;
	origin.x = 0;
	origin.y = 0;
    SDL_RenderCopyExF(renderer, bullet->texture, NULL, &bullet->rect,
                      bullet->rotation, &bullet->textureCentre, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderDrawRectF(renderer, &(bullet->rect));
    
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
	printf("%f ", rotation);
	if(rotation < 0)
		rotation += 360;

	printf("%f\n", rotation);

	rotation *= DEG2RAD;
	float w = bullet->rect.w;
	float h = bullet->rect.h;
	float xoff = bullet->textureCentre.x / 2;
	float yoff = bullet->textureCentre.y / 2;
	float c = cos(rotation);
	float s = sin(rotation);

	/* bullet->corners[0].x = (-xoff)*c - (-yoff)*s; */
	/* bullet->corners[0].y = (-xoff)*s + (-yoff)*c; */

	/* bullet->corners[1].x = (-xoff + w)*c - (-yoff)*s; */
	/* bullet->corners[1].y = (-xoff + w)*s + (-yoff)*c; */

	/* bullet->corners[2].x = (-xoff + w)*c - (-yoff + h)*s; */
	/* bullet->corners[2].y = (-xoff + w)*s + (-yoff + h)*c; */

	/* bullet->corners[3].x = (-xoff)*c - (-yoff + h)*s; */
	/* bullet->corners[3].y = (-xoff)*s + (-yoff + h)*c; */

	bullet->corners[0].x = 0;
	bullet->corners[0].y = 0;

	bullet->corners[1].x = w*cos(rotation);
	bullet->corners[1].y = w*sin(rotation);

	bullet->corners[2].x = w*cos(rotation) - h*sin(rotation);
	bullet->corners[2].y = w*sin(rotation) + h*cos(rotation);

	bullet->corners[3].x = 0 - h*sin(rotation);
	bullet->corners[3].y = h*cos(rotation);
}

