#ifndef MAIN_STRUCT_H
# define MAIN_STRUCT_H

const int WIDTH = 800;
const int HEIGHT = 600;


typedef struct S_SDL_data {
    SDL_Window  *window;
    SDL_Renderer* renderer;
    SDL_Event   windowEvent;
}   t_sdl_data;
#endif