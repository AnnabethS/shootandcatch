#include <SDL2/SDL.h>

#ifndef ANNA_BULLET_HEADER
#define ANNA_BULLET_HEADER

typedef struct bullet_t
{
    SDL_FRect rect;
    SDL_Texture* texture;
    SDL_FPoint textureCentre;
    SDL_FPoint perFrameMove;
    float speed;
    float rotation;
    struct bullet_t* next;
}bullet_t;

void initBullet(bullet_t* bullet, float rotation,
                SDL_Texture* texture, float startX, float startY);

void moveBullet(bullet_t* bullet);

void updateBullet(bullet_t* bullet);

void drawBullet(bullet_t* bullet, SDL_Renderer* renderer);
#endif
