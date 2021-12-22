#include <SDL2/SDL.h>

#ifndef ANNA_PLAYER_HEADER
#define ANNA_PLAYER_HEADER
typedef struct 
{
    SDL_Rect rect;
    float rotation;
    SDL_Texture* defaultTexture;
    SDL_Point textureCentre;
}player_t;

// this function will malloc() the player on the heap, and initialise it
player_t* initPlayer(float xStart, float yStart, float rotation,
                     SDL_Texture* texture, float textureScale);
void drawPlayer(SDL_Renderer* renderer, player_t* player);

#endif
