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
    imageSurface = NULL;
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

void displayText(SDL_Renderer* renderer, char *text, int x, int y, int size_font, const SDL_Color textColor)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect destRect;

    font = TTF_OpenFont("ARIAL.TTF", size_font);
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destRect.x = x;
    destRect.y = y;
    destRect.w = (surface->w > WIDTH) ? WIDTH - 1 : surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void display_text_batch(SDL_Renderer *renderer, char *file, const int START_HEIGHT)
{
    clear_rect_screen(renderer, START_HEIGHT);
    displayText(renderer, file, 0, START_HEIGHT, 25, (const SDL_Color){255, 255, 255, 255});
    clear_rect_screen(renderer, START_HEIGHT + 50);
    displayText(renderer, get_bck_slash(file), 0, START_HEIGHT + 50, 25, (const SDL_Color){255, 255, 255, 255});
    clear_rect_screen(renderer, START_HEIGHT + 100);
}

void draw_halfs(SDL_Renderer *renderer, const int bck[4], const SDL_Rect half, char *str)
{
    SDL_SetRenderDrawColor(renderer, bck[0], bck[1], bck[2], bck[3]);
    SDL_RenderFillRect(renderer, &half);
    displayTextOnRect(renderer, str, half, 30, (const SDL_Color){255, 255, 255, 255});
}


void draw_rect(SDL_Renderer *renderer, SDL_Rect *rect, t_pos pos, t_pos size, const int rgba[4], int fill)
{
    rect->x = pos.x;
    rect->y = pos.y;
    rect->w = size.x - pos.x;
    rect->h = size.y - pos.y;
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderDrawRect(renderer, rect);
    if (fill)
        SDL_RenderFillRect(renderer, rect);
}

void displayTextOnRect(SDL_Renderer* renderer, char *text, SDL_Rect destRect, int size_font, const SDL_Color textColor)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;

    font = TTF_OpenFont("ARIAL.TTF", size_font);
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destRect.w = surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void clear_rect_screen(SDL_Renderer* renderer, int clearHeight)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect clearRect = {0, clearHeight, WIDTH, 50};
    SDL_RenderFillRect(renderer, &clearRect);
    SDL_RenderPresent(renderer);
}