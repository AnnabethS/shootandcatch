#include <SDL2/SDL.h>

#ifndef ANNA_BULLET_HEADER
#define ANNA_BULLET_HEADER

typedef struct bullet_t
{
    SDL_FRect rect;
    SDL_FPoint textureCentre;
    SDL_FPoint perFrameMove;
    float speed;
    float rotation;
	int currentBounces;
	//corners are defined topleft topright botright botleft
	SDL_FPoint corners[4];
    struct bullet_t* next;
}bullet_t;

void initBullet(bullet_t* bullet, float rotation,
                SDL_Texture* texture, float startX, float startY);

void moveBullet(bullet_t* bullet);

int updateBullet(bullet_t* bullet, int screenWidth, int screenHeight);

void drawBullet(bullet_t* bullet, SDL_Renderer* renderer);

void rotateBulletCorners(bullet_t *bullet, float rotation);
void rotateFPoint(SDL_FPoint* point, SDL_FPoint* centre, float rotation);
#endif
