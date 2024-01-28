#include "main.h"
#include "main_struct.h"

void window_handling(t_sdl_data *p)
{
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
        }
        SDL_Delay(16); 
    }
}

int main(int argc, char **argv)
{
    t_sdl_data  p;
    SDL_Window  *window;
    SDL_Event   windowEvent;
    SDL_Init(SDL_INIT_EVERYTHING);

    p.window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!p.window)
    {
        fprintf(stderr, "Could not create window");
        return (1);
    }
    p.renderer = SDL_CreateRenderer(p.window, -1, SDL_RENDERER_ACCELERATED);
    window_handling(&p);
    SDL_DestroyWindow(p.window);
    SDL_Quit();
    return (0);
}
