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

SDL_Texture *load_image(SDL_Renderer* renderer, const char* path)
{
    SDL_Texture *imageTexture;
    SDL_Surface *imageSurface;

    imageSurface = IMG_Load(path);
    if (!imageSurface)
    {
        fprintf(stderr, "Unable to load image SDL_image: %s\n", IMG_GetError());
        return (NULL);
    }
    imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    return (imageTexture);
}

void place_image(SDL_Renderer* renderer, t_sdl_image *i)
{
    SDL_Rect destinationRect;

    destinationRect.x = i->cur_offset.x;
    destinationRect.y = i->cur_offset.y;
    SDL_QueryTexture(i->imageTexture, NULL, NULL, &destinationRect.w, &destinationRect.h);
    destinationRect.w += i->cur_spacing * i->ZOOMFACTOR;
    destinationRect.h += i->cur_spacing * i->ZOOMFACTOR;
    SDL_RenderCopy(renderer, i->imageTexture, NULL, &destinationRect);
}

void get_image_bottom_left(t_sdl_image *i, t_pos *bottom_right)
{
    int w;
    int h;

    SDL_QueryTexture(i->imageTexture, NULL, NULL, &w, &h);
    bottom_right->x = i->cur_offset.x + w + (i->cur_spacing * i->ZOOMFACTOR);
    bottom_right->y = i->cur_offset.y + h + (i->cur_spacing * i->ZOOMFACTOR);
}