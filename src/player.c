#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "player.h"

player_t* initPlayer(float xStart, float yStart, float rotation,
                     SDL_Texture* texture, float textureScale)
{
    player_t* p = malloc(sizeof(player_t));
    p->rect.x = xStart;
    p->rect.y = yStart;
    p->defaultTexture = texture;
    SDL_QueryTexture(texture, NULL, NULL, &p->rect.w, &p->rect.h);
    p->rect.w *= textureScale;
    p->rect.h *= textureScale;
    p->rotation = 0;
    p->textureCentre.x = p->rect.w / 2;
    p->textureCentre.y = p->rect.h / 2;
    return p;
}

void drawPlayer(SDL_Renderer *renderer, player_t* player)
{
    SDL_RenderCopyEx(renderer, player->defaultTexture, NULL, &player->rect,
                     player->rotation, &player->textureCentre, SDL_FLIP_NONE);
}
