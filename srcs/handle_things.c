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
