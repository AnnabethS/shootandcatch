#include <SDL2/SDL_image.h>

#ifndef TEXTURES
#define TEXTURES


typedef struct{
	SDL_Texture* bullet;
	SDL_Texture* player;
	SDL_Texture* heart;
}ts;


char loadTextures(SDL_Renderer* r);

#endif

extern ts textures;
