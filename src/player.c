#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include "player.h"

#define PI 3.14159265

player_t* initPlayer(float xStart, float yStart, float rotation,
                     SDL_Texture* texture, float textureScale)
{
    player_t* p = malloc(sizeof(player_t));
    p->rect.x = xStart;
    p->rect.y = yStart;
    p->defaultTexture = texture;
    int w,h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    p->rect.w = (int)w * textureScale;
    p->rect.h = (int)h * textureScale;
    p->rotation = 0;
    p->textureCentre.x = p->rect.w / 2;
    p->textureCentre.y = p->rect.h / 2;
    return p;
}

void drawPlayer(SDL_Renderer *renderer, player_t* player)
{
    printf("rot %f\n", player->rotation);
    SDL_RenderCopyExF(renderer, player->defaultTexture, NULL, &player->rect,
                     player->rotation, &player->textureCentre, SDL_FLIP_NONE);
}

void movePlayer(player_t *player, int moveX, int moveY)
{
    float actualMoveX, actualMoveY;
    if(moveX != 0 && moveY != 0)
    {
        actualMoveX = (float)moveX * player->speed * sqrt(2);
        actualMoveY = (float)moveY * player->speed * sqrt(2);
    }
    else
    {
        actualMoveX = moveX * player->speed;
        actualMoveY = moveY * player->speed;
    }

    player->rect.x = player->rect.x + (moveX * player->speed);
    player->rect.y = player->rect.y + (moveY * player->speed);
}

void rotatePlayer(player_t *player, int mouseX, int mouseY)
{
    player->rotation = (atan2((player->rect.y + player->textureCentre.y/2) - mouseY,
                              (player->rect.x + player->textureCentre.x/2) - mouseX)
                        * 180/PI);
    if(player->rotation >= 360)
        player->rotation = 360 - player->rotation;
    else if(player->rotation < 0)
        player->rotation += 360;
    player->rotation -= 90;
}
