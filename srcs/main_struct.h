#ifndef MAIN_STRUCT_H
# define MAIN_STRUCT_H

const int WIDTH = 800;
const int HEIGHT = 600;


typedef struct S_SDL_data {
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Event       windowEvent;
}   t_sdl_data;

typedef struct S_SDL_Image {
    SDL_Surface     *imageSurface;
    SDL_Texture     *imageTexture;
}   t_sdl_image;

typedef struct S_Track_mouse {
    int down_posX;
    int down_posY;
    int current_posX;
    int current_posY;
    int up_posX;
    int up_posY;
    SDL_Rect draw_rect;
}   t_track_mouse;

#endif