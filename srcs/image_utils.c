#include "main.h"
#include "main_struct.h"

t_pos *current_pos(t_pos *pos)
{
    static t_pos *old_pos;

    if (!pos)
        return (old_pos);
    old_pos = pos;
}

SDL_Texture* load_image(SDL_Renderer* renderer, const char* path)
{
	SDL_Texture *imageTexture;
    SDL_Surface *imageSurface;
	
	imageSurface = IMG_Load(path);
    if (!imageSurface)
	{
        fprintf(stderr, "Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return (NULL);
    }
    imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    return (imageTexture);
}

void place_image(SDL_Renderer* renderer, SDL_Texture* imageTexture, int cur_spacing)
{
	t_pos *change_pts;
    SDL_Rect destinationRect;

	change_pts = current_pos(NULL);
	destinationRect.x = change_pts->x;
	destinationRect.y = change_pts->y;
    SDL_QueryTexture(imageTexture, NULL, NULL, &destinationRect.w, &destinationRect.h);
	destinationRect.w += cur_spacing;
	destinationRect.h += cur_spacing;
    SDL_RenderCopy(renderer, imageTexture, NULL, &destinationRect);
}