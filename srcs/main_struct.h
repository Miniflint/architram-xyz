#ifndef MAIN_STRUCT_H
# define MAIN_STRUCT_H

typedef struct S_pos {
    int x;
    int y;
}   t_pos;

typedef struct S_pos_triple {
    float x;
    float y;
    float z;
}   t_pos_triple;

typedef struct S_SDL_data {
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Event       windowEvent;
}   t_sdl_data;

typedef struct S_SDL_Image {
    SDL_Surface     *imageSurface;
    SDL_Texture     *imageTexture;
    TTF_Font*       font;
    t_pos   cur_offset;
    t_pos   image_dimensions;
    int     cur_spacing;
    int     ZOOMFACTOR;
}   t_sdl_image;


typedef struct S_Track_mouse {
    t_pos   down_pos;
    t_pos   current_pos;
    t_pos   last_pos;
    t_pos   up_pos;
    SDL_Rect draw_rect;
    SDL_Rect draw_rect_delete;
    t_pos   before_offset;
    t_pos   after_offset;
}   t_track_mouse;

typedef struct S_File_content {
	char	*path;
    char    *str;
    char    *trimmed_str;
    int         line_len;
    long int    size;
    long int    real_size;
    int         n_pts;
    t_pos_triple **pos;
}   t_file;

typedef struct S_Drag_Drop {
    SDL_Rect jpg_box;
    SDL_Rect xyz_box;
    char *path_jpg;
    char *path_xyz;
}   t_drag;

typedef struct S_all {
    t_sdl_data *data;
    t_sdl_image *image;
    t_file *file;
    t_drag *drag;
}   t_all;

/* utils struct */
t_file *__init_file(char *path);
t_pos   *current_pos(t_pos *pos);
void    get_image_bottom_left(t_sdl_image *i, t_pos *bottom_right);
void    place_image(SDL_Renderer* renderer, t_sdl_image *i);

void handle_mouse(t_sdl_data *p, t_sdl_image *i, t_track_mouse *mouse, int *track_mouse);
int handle_key_event(t_sdl_data *p, t_sdl_image *i);

#endif