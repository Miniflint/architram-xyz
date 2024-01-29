#ifndef MAIN_STRUCT_H
# define MAIN_STRUCT_H

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

typedef struct S_File_content {
    char    *str;
    char    *trimmed_str;
    int         line_len;
    long int    size;
    long int    real_size;
    int         n_pts;
    float       **float_array;
}   t_file;

#endif