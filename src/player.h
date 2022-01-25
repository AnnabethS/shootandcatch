#include <SDL2/SDL.h>

#ifndef ANNA_PLAYER_HEADER
#define ANNA_PLAYER_HEADER
typedef struct player_t
{
    SDL_FRect rect;
    float rotation;
    SDL_Texture* defaultTexture;
    SDL_FPoint textureCentre;
    float speed;
	int currentHealth;
	int maxHealth;
}player_t;

// this function will malloc() the player on the heap, and initialise it
player_t* initPlayer(float xStart, float yStart, float rotation,
                     SDL_Texture* texture, float textureScale);
void drawPlayer(SDL_Renderer* renderer, player_t* player);

void movePlayer(player_t *player, int moveX, int moveY);
void rotatePlayer(player_t *player, int mouseX, int mouseY);

#endif
