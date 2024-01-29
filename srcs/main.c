#include "main.h"
#include "main_struct.h"

void print_mouse(char *str, int x, int y)
{
    printf("%s (%d, %d)\n", str, x, y);
}

void window_handling(t_sdl_data *p, t_sdl_image *i)
{
    int             track_mouse;
    t_track_mouse   mouse;

    track_mouse = 0;
    while (1)
    {
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
                    mouse.down_posX = p->windowEvent.button.x;
                    mouse.down_posY = p->windowEvent.button.y;
                    mouse.draw_rect.x = mouse.down_posX;
                    mouse.draw_rect.y = mouse.down_posY;
                }
            }
            else if (p->windowEvent.type == SDL_MOUSEBUTTONUP)
            {
                if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
                {
                    track_mouse = 0;
                    mouse.up_posX = p->windowEvent.button.x;
                    mouse.up_posY = p->windowEvent.button.y;
                }
            }
            if (track_mouse == 1)
            {
                mouse.draw_rect.w = p->windowEvent.button.x - mouse.down_posX;
                mouse.draw_rect.h = p->windowEvent.button.y - mouse.down_posY;
                SDL_RenderDrawRect(p->renderer, &mouse.draw_rect);
                SDL_RenderFillRect(p->renderer, &mouse.draw_rect);
                SDL_RenderPresent(p->renderer);
                SDL_GetMouseState(&mouse.current_posX, &mouse.current_posY);
            }
        }
        SDL_Delay(2);
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
    p.renderer = SDL_CreateRenderer(p.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(p.renderer, 255, 0, 0, 255);
    window_handling(&p, &i);
    SDL_DestroyWindow(p.window);
    SDL_Quit();
    return (0);
}
