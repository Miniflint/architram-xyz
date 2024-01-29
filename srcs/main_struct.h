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

typedef struct S_pos {
    int x;
    int y;
}   t_pos;

typedef struct S_Track_mouse {
    t_pos   down_pos;
    t_pos   current_pos;
    t_pos   last_pos;
    t_pos   up_pos;
    SDL_Rect draw_rect;
    SDL_Rect draw_rect_delete;
}   t_track_mouse;

#endif