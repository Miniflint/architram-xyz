#include "main.h"
#include "main_struct.h"

t_pos *current_pos(t_pos *pos)
{
    static t_pos *new_pos;

    if (!pos)
        return (new_pos);
    new_pos = pos;
    return (new_pos);
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

void place_image(SDL_Renderer* renderer, t_sdl_image *i)
{
    t_pos *change_pts;
    SDL_Rect destinationRect;

    change_pts = current_pos(NULL);
    destinationRect.x = change_pts->x;
    destinationRect.y = change_pts->y;
    SDL_QueryTexture(i->imageTexture, NULL, NULL, &destinationRect.w, &destinationRect.h);
    destinationRect.w += (i->cur_spacing * i->ZOOMFACTOR);
    destinationRect.h += (i->cur_spacing * i->ZOOMFACTOR);
    SDL_RenderCopy(renderer, i->imageTexture, NULL, &destinationRect);
}

void get_image_bottom_left(t_sdl_image *i, t_pos *bottom_right)
{
	t_pos *base_pts;
    int w;
    int h;

	base_pts = current_pos(NULL);
    SDL_QueryTexture(i->imageTexture, NULL, NULL, &w, &h);
    bottom_right->x = base_pts->x + w + (i->cur_spacing * i->ZOOMFACTOR);
    bottom_right->y = base_pts->y + h + (i->cur_spacing * i->ZOOMFACTOR);
}