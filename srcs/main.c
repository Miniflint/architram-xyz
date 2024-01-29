#include "main.h"
#include "main_struct.h"

void print_mouse(char *str, int x, int y)
{
    printf("%s (%d, %d)\n", str, x, y);
}

void draw_rect(t_sdl_data *p, SDL_Rect *rect, t_pos pos, t_pos size, const int rgba[4])
{
    rect->x = pos.x;
    rect->y = pos.y;
    rect->w = size.x - pos.x;
    rect->h = size.y - pos.y;
    SDL_SetRenderDrawColor(p->renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderDrawRect(p->renderer, rect);
    SDL_RenderFillRect(p->renderer, rect);
}

void window_handling(t_sdl_data *p, t_sdl_image *i)
{
    int             track_mouse;
    t_track_mouse   mouse;
	Uint64 start;
    double elapsed;
    const int white_draw[4] = 
        {255, 255, 255, 100};
    const int black_draw[4] = 
        {0, 0, 0, 255};

    track_mouse = 0;
    mouse.current_pos.x = 0;
    mouse.current_pos.y = 0;
    while (1)
    {
        start = SDL_GetPerformanceCounter();
        if (SDL_PollEvent(&p->windowEvent))
        {
            if (p->windowEvent.type == SDL_QUIT)
                break ;
            if (p->windowEvent.type == SDL_KEYDOWN)
            {
                if (p->windowEvent.key.keysym.sym == SDLK_ESCAPE)
                    break ;
            }
            else if (p->windowEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
                {
                    track_mouse = 1;
                    mouse.down_pos.x = p->windowEvent.button.x;
                    mouse.down_pos.y = p->windowEvent.button.y;
                }
            }
            else if (p->windowEvent.type == SDL_MOUSEBUTTONUP)
            {
                if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
                {
                    track_mouse = 0;
                    mouse.up_pos.x = p->windowEvent.button.x;
                    mouse.up_pos.y = p->windowEvent.button.y;
                    draw_rect(p, &mouse.draw_rect_delete, (t_pos){0, 0}, (t_pos){WIDTH, HEIGHT}, black_draw);
                }
            }
            if (track_mouse == 1)
            {
                mouse.last_pos.x = mouse.current_pos.x;
                mouse.last_pos.y = mouse.current_pos.y;
                SDL_GetMouseState(&mouse.current_pos.x, &mouse.current_pos.y);
                if (mouse.last_pos.x)
                    draw_rect(p, &mouse.draw_rect_delete, mouse.down_pos, mouse.last_pos, black_draw);
                draw_rect(p, &mouse.draw_rect, mouse.down_pos, mouse.current_pos, white_draw);
            }
        }
        SDL_RenderPresent(p->renderer);
        elapsed = (SDL_GetPerformanceCounter() - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
	    SDL_Delay(floor(16.66666f - elapsed));
    }
}

int main(int argc, char **argv)
{
    t_sdl_data  p;
    t_sdl_image i;

    SDL_Init(SDL_INIT_EVERYTHING);
    p.window = SDL_CreateWindow("Tif visual", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!p.window)
    {
        fprintf(stderr, "Could not create window");
        return (1);
    }
    p.renderer = SDL_CreateRenderer(p.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(p.renderer, SDL_BLENDMODE_BLEND);
    window_handling(&p, &i);
    SDL_DestroyWindow(p.window);
    SDL_Quit();
    return (0);
}
