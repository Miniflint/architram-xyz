#include "main.h"
#include "main_struct.h"

int handle_key_event(t_sdl_data *p, t_sdl_image *i)
{
    if (p->windowEvent.type == SDL_KEYDOWN)
    {
        if (p->windowEvent.key.keysym.sym == SDLK_ESCAPE)
            return (1);
        if (p->windowEvent.key.keysym.sym == SDLK_r)
        {
            i->cur_offset.x = 0;
            i->cur_offset.y = 0;
            i->cur_spacing = 1;
        }
    }
    return (0);
}

void handle_mouse_down(t_sdl_data *p, t_track_mouse *mouse, int *track_mouse)
{
    if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
    {
        *track_mouse = 1;
        mouse->down_pos.x = p->windowEvent.button.x;
        mouse->down_pos.y = p->windowEvent.button.y;
    }
    else if (p->windowEvent.button.button == SDL_BUTTON_RIGHT)
    {
        *track_mouse = 2;
        mouse->before_offset.x = p->windowEvent.button.x;
        mouse->before_offset.y = p->windowEvent.button.y;
    }
}

void handle_mouse_up(t_sdl_data *p, t_track_mouse *mouse, int *track_mouse)
{
    if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
    {
        *track_mouse = 0;
        mouse->up_pos.x = p->windowEvent.button.x;
        mouse->up_pos.y = p->windowEvent.button.y;
    }
    else if (p->windowEvent.button.button == SDL_BUTTON_RIGHT)
    {
        *track_mouse = 0;
    }
}

void handle_wheel(t_sdl_data *p, t_sdl_image *i, t_track_mouse *mouse)
{
    SDL_GetMouseState(&mouse->current_pos.x, &mouse->current_pos.y);
    i->cur_spacing += p->windowEvent.wheel.y;
    i->cur_offset.x -= ((float)mouse->current_pos.x / WIDTH) * (i->ZOOMFACTOR * p->windowEvent.wheel.preciseY);
    i->cur_offset.y -= ((float)mouse->current_pos.y / HEIGHT) * (i->ZOOMFACTOR * p->windowEvent.wheel.preciseY);
}

void handle_mouse(t_sdl_data *p, t_sdl_image *i, t_track_mouse *mouse, int *track_mouse)
{
    if (p->windowEvent.type == SDL_MOUSEBUTTONDOWN)
    {
        handle_mouse_down(p, mouse, track_mouse);
    }
    else if (p->windowEvent.type == SDL_MOUSEBUTTONUP)
    {
        handle_mouse_up(p, mouse, track_mouse);
    }
    else if (p->windowEvent.type == SDL_MOUSEWHEEL)
    {
        handle_wheel(p, i, mouse);
    }
}


int handle_thread_load_jpg(void *all)
{
    t_all *a;

    a = (t_all *)all;
    displayText(a->data->renderer, "[INFO]: File loading", 0, DISPLAY_LOADED, 25, (const SDL_Color){255, 255, 0, 255});
    a->image->imageTexture = load_image(a->data->renderer, a->drag->path_jpg);
    clear_rect_screen(a->data->renderer, DISPLAY_LOADED);
    if (!a->image->imageTexture)
    {
        displayText(a->data->renderer, (char *)IMG_GetError(), 0, DISPLAY_LOADED, 25, (const SDL_Color){255, 0, 0, 255});
        return (1);
    }
    displayText(a->data->renderer, "[INFO]: File loaded", 0, DISPLAY_LOADED, 25, (const SDL_Color){0, 255, 0, 255});
    return (0);
}

int handle_thread_load_xyz(void *all)
{
    t_all *a;

    a = (t_all *)all;
    displayText(a->data->renderer, "[INFO]: File loading", 0, (HEIGHT / 2) + DISPLAY_LOADED, 25, (const SDL_Color){255, 255, 0, 255});
    a->file = __init_file(a->drag->path_xyz);
    clear_rect_screen(a->data->renderer, (HEIGHT / 2) + DISPLAY_LOADED);
    if (!a->file)
    {
        displayText(a->data->renderer, "Unsupported file format", 0, (HEIGHT / 2) + DISPLAY_LOADED, 25, (const SDL_Color){255, 0, 0, 255});
        return (1);
    }
    displayText(a->data->renderer, "[INFO]: File loaded", 0, (HEIGHT / 2) + DISPLAY_LOADED, 25, (const SDL_Color){0, 255, 0, 255});
    return (0);
}


void handle_drop_file(t_all *all, char *file, SDL_Thread *threads[2])
{
    if (all->data->windowEvent.type == SDL_DROPFILE)
    {
        if (custom_strstr(file, ".tif") == 0 || custom_strstr(file, ".tiff") == 0)
        {
            display_text_batch(all->data->renderer, file, DISPLAY_FULLPATH_FILE);
            displayText(all->data->renderer, "Unsupported file format: TIFF/TIF", 0, DISPLAY_LOADED, 25, (const SDL_Color){255, 0, 0, 255});
            SDL_free(file);
        }
        else if (
            custom_strstr(file, ".jpg") == 0 ||
            custom_strstr(file, ".png") == 0 ||
            custom_strstr(file, ".bmp") == 0 ||
            custom_strstr(file, ".pnm") == 0 ||
            custom_strstr(file, ".xpm") == 0
        )
        {
            if (all->drag->path_jpg)
            {
                SDL_DestroyTexture(all->image->imageTexture);
                all->image->imageTexture = NULL;
                SDL_free(all->drag->path_jpg);
            }
            all->drag->path_jpg = file;
            draw_halfs(all->data->renderer, all->text_colors, all->top_half, "Drop your IMAGE");
            display_text_batch(all->data->renderer, all->drag->path_jpg, DISPLAY_FULLPATH_FILE);
            threads[0] = SDL_CreateThread(handle_thread_load_jpg, "random", all);
        }
        else
        {
            if (all->drag->path_xyz)
            {
                free_double_array((void **)all->file->pos, all->file->n_pts);
                SDL_free(all->drag->path_xyz);
                free(all->file->str);
                free(all->file);
            }
            all->drag->path_xyz = file;
            draw_halfs(all->data->renderer, all->text_colors, all->bot_half, "Drop your xyz file");
            display_text_batch(all->data->renderer, all->drag->path_xyz, HEIGHT / 2 + DISPLAY_FULLPATH_FILE);
            threads[1] = SDL_CreateThread(handle_thread_load_xyz, "random2", all);
        }
    }
}