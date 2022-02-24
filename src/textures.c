#include "textures.h"
#include "anna-layer.h"

local char loadSingleTexture(SDL_Renderer* r, char* path, SDL_Texture** dest);

//returns 0 on successful load, 1 on unsuccessful
char loadTextures(SDL_Renderer *r)
{
	char errors = 0;
	errors += loadSingleTexture(r, "res/player.png", &textures.player);
	errors += loadSingleTexture(r, "res/bullet.png", &textures.bullet);
	errors += loadSingleTexture(r, "res/heart.png", &textures.heart);
	return errors > 0;
}


local char loadSingleTexture(SDL_Renderer* r, char* path, SDL_Texture** dest)
{
	SDL_Surface *tmp = IMG_Load(path);
	if(tmp == NULL)
	{
		printf("failed loading img: %s\nexiting...", path);
		return 1;
	}
	*dest = SDL_CreateTextureFromSurface(r, tmp);
	SDL_FreeSurface(tmp);
	if(*dest == NULL)
	{
		printf("failed creating texture: %s\nexiting...", path);
		return 1;
	}
	return 0;
}
