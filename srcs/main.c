#include "main.h"
#include "main_struct.h"
#include "parse_file.h"

void displayText(SDL_Renderer* renderer, char *text, int x, int y)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect destRect;
    const SDL_Color textColor
        = {255, 255, 255, 255};

    font = TTF_OpenFont("ARIAL.TTF", 50);
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destRect.x = x;
    destRect.y = y;
    destRect.w = surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_rect(t_sdl_data *p, SDL_Rect *rect, t_pos pos, t_pos size, const int rgba[4], int fill)
{
    rect->x = pos.x;
    rect->y = pos.y;
    rect->w = size.x - pos.x;
    rect->h = size.y - pos.y;
    SDL_SetRenderDrawColor(p->renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderDrawRect(p->renderer, rect);
    if (fill)
        SDL_RenderFillRect(p->renderer, rect);
}

/* 
 * mx = mouse position x relative to screen 
 * my = mouse position x relative to screen
 * W = constant Width (of the screen)
 * H = constant Height (of the screen)
 * Z = constant Zoom factor (100)
 * d = direction of zoom (1 or -1)
 * 
 * offset_x = mx / W * Z * d
 * offset_y = my / H * Z * d
 */


void render_track(t_sdl_data *p, t_sdl_image *i, t_track_mouse *mouse, int *track_mouse)
{
    const int black_draw[4] = {0, 0, 0, 100};
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
    place_image(p->renderer, i);
    if (*track_mouse == 1)
    {
        SDL_GetMouseState(&mouse->current_pos.x, &mouse->current_pos.y);
        get_image_bottom_left(i, &(i->image_dimensions));
        draw_rect(p, &mouse->draw_rect, mouse->down_pos, mouse->current_pos, black_draw, 1);
    }
    else if (*track_mouse == 2)
    {
        mouse->after_offset.x = p->windowEvent.button.x;
        mouse->after_offset.y = p->windowEvent.button.y;
        i->cur_offset.x += mouse->after_offset.x - mouse->before_offset.x;
        i->cur_offset.y += mouse->after_offset.y - mouse->before_offset.y;
        mouse->before_offset = mouse->after_offset;
    }
}

void window_handling_cal(t_sdl_data *p, t_sdl_image *i, t_file *f)
{
    int             track_mouse;
    t_track_mouse   mouse;

    track_mouse = 0;
    i->cur_offset.x = 0;
    i->cur_offset.y = 0;
    if (f)
        printf("hi\n");
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
    current_pos(&(i->cur_offset));
    i->cur_spacing = 1;
    place_image(p->renderer, i);
    while (1)
    {
        if (SDL_PollEvent(&p->windowEvent))
        {
            if (p->windowEvent.type == SDL_QUIT)
                break ;
            if (handle_key_event(p, i))
                break ;
            handle_mouse(p, i, &mouse, &track_mouse);
    		render_track(p, i, &mouse, &track_mouse);
            SDL_RenderPresent(p->renderer);
        }
    }
	SDL_DestroyTexture(i->imageTexture);
	SDL_DestroyRenderer(p->renderer);
    SDL_DestroyWindow(p->window);
}

int SDL_main(int argc, char **argv)
{
    t_sdl_data  p;
    t_sdl_image i;
    t_file *f;

    if (argc < 2)
    {
        fprintf(stderr, "Provide a .jpg file please");
        return (1);
    }
    i.cur_offset.x = 0;
    i.cur_offset.y = 0;
    SDL_Init(SDL_INIT_EVENTS);  
    TTF_Init();
    p.window = SDL_CreateWindow("JPG visuals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!p.window)
    {
        fprintf(stderr, "Could not create window");
        return (1);
    }
    p.renderer = SDL_CreateRenderer(p.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(p.renderer, SDL_BLENDMODE_BLEND);
    displayText(p.renderer, "LOADING FILE", 0, 0);
    //handle_drop(&p, &i);
	i.imageTexture = load_image(p.renderer, argv[1]);
	if (!i.imageTexture)
		return (1);
    i.ZOOMFACTOR = 100;
    f = __init_file(NULL);
    //if (!f)
    //    return (1);
    window_handling_cal(&p, &i, f);
    SDL_Quit();
    //free(f->str);
    return (0);
}
