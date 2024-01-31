#include "main.h"
#include "main_struct.h"

SDL_Texture* load_image(SDL_Renderer* renderer, const char* path)
{
	SDL_Texture* imageTexture;
    SDL_Surface* imageSurface;
	
	imageSurface = IMG_Load(path);
    if (!imageSurface)
	{
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }
    imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    return (imageTexture);
}

void place_image(SDL_Renderer* renderer, SDL_Texture* imageTexture, int x, int y)
{
    // Set the destination rectangle
    const SDL_Rect destinationRect
		= {x, y, 0, 0};

    // Query the texture for its width and height
    SDL_QueryTexture(imageTexture, NULL, NULL, &destinationRect.w, &destinationRect.h);

    // Render the texture at the specified position
    SDL_RenderCopy(renderer, imageTexture, NULL, &destinationRect);
}