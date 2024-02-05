#include "main.h"
#include "main_struct.h"
#include "parse_file.h"



/* 
 * mx = mouse position x relative to screen 
 * my = mouse position x relative to screen
 * W = constant Width (of the screen)
 * H = constant Height (of the screen)
 * Z = constant Zoom factor (100)
 * d = direction of zoom (1 or -1)
 * 
 * offset_x = mx / W * Z * d
 * offset_y = my / H * Z * d
 */


void render_track(t_sdl_data *p, t_sdl_image *i, t_track_mouse *mouse, int *track_mouse)
{
    const int black_draw[4] = {0, 0, 0, 100};
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
    place_image(p->renderer, i);
    if (*track_mouse == 1)
    {
        SDL_GetMouseState(&mouse->current_pos.x, &mouse->current_pos.y);
        get_image_bottom_left(i, &(i->image_dimensions));
        draw_rect(p->renderer, &mouse->draw_rect, mouse->down_pos, mouse->current_pos, black_draw, 1);
    }
    else if (*track_mouse == 2)
    {
        mouse->after_offset.x = p->windowEvent.button.x;
        mouse->after_offset.y = p->windowEvent.button.y;
        i->cur_offset.x += mouse->after_offset.x - mouse->before_offset.x;
        i->cur_offset.y += mouse->after_offset.y - mouse->before_offset.y;
        mouse->before_offset = mouse->after_offset;
    }
}

void window_handling_cal(t_sdl_data *p, t_sdl_image *i, t_file *f)
{
    int             track_mouse;
    t_track_mouse   mouse;

    track_mouse = 0;
    i->cur_offset.x = 0;
    i->cur_offset.y = 0;
    // change that so it parse the numbers after
    f = f;
    current_pos(&(i->cur_offset));
    i->cur_spacing = 1;
    place_image(p->renderer, i);
    while (1)
    {
        if (SDL_PollEvent(&p->windowEvent))
        {
            if (p->windowEvent.type == SDL_QUIT)
                break ;
            if (handle_key_event(p, i))
                break ;
            handle_mouse(p, i, &mouse, &track_mouse);
    		render_track(p, i, &mouse, &track_mouse);
            SDL_RenderPresent(p->renderer);
        }
    }
	SDL_DestroyTexture(i->imageTexture);
	SDL_DestroyRenderer(p->renderer);
    SDL_DestroyWindow(p->window);
}

// TODO: make displayed text disapear between "file loading" and "file loaded" / error


void assign_value(t_all *all, t_sdl_data *p, t_drag *d, t_sdl_image *i)
{
    all->top_half.x = 0;
    all->top_half.y = 0;
    all->top_half.w = WIDTH;
    all->top_half.h = HEIGHT / 2;

    all->bot_half.x = 0;
    all->bot_half.y = HEIGHT / 2;
    all->bot_half.w = WIDTH;
    all->bot_half.h = HEIGHT / 2;

    all->text_colors[0] = 0;
    all->text_colors[1] = 0;
    all->text_colors[2] = 0;
    all->text_colors[3] = 255;

    all->data = p;
    all->drag = d;
    all->image = i;
    all->drag->path_jpg = NULL;
    all->drag->path_xyz = NULL;
}

t_file *drag_drop_window(t_sdl_data *p, t_drag *drag, t_sdl_image *i)
{
    t_all all;

    assign_value(&all, p, drag, i);
    SDL_Thread *threads[2];
    threads[0] = NULL;
    threads[1] = NULL;
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
    draw_halfs(p->renderer, all.text_colors, all.top_half, "Drop your IMAGE");
    draw_halfs(p->renderer, all.text_colors, all.bot_half, "Drop your xyz file");
    while (1)
    {
        if (SDL_PollEvent(&p->windowEvent))
        {
            if (p->windowEvent.type == SDL_QUIT)
                break ;
            if (handle_key_event(all.data, all.image))
                break ;
            handle_drop_file(&all, p->windowEvent.drop.file, threads);
        }
        SDL_Delay(16.6666f);
    }
    SDL_WaitThread(threads[0], NULL);
    SDL_WaitThread(threads[1], NULL);
    SDL_DetachThread(threads[0]);
    SDL_DetachThread(threads[1]);
    //SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    //SDL_RenderClear(p->renderer);
    place_image(all.data->renderer, all.image);
    return (all.file);
}

int SDL_main(int argc, char *argv[])
{
    t_sdl_data  p;
    t_sdl_image i;
    t_file *f;
    t_drag drag;

    argc = argc;
    argv = argv;
    i.cur_offset.x = 0;
    i.cur_offset.y = 0;
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();
    p.window = SDL_CreateWindow("JPG visuals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!p.window)
    {
        fprintf(stderr, "Could not create window");
        return (1);
    }
    p.renderer = SDL_CreateRenderer(p.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(p.renderer, SDL_BLENDMODE_BLEND);    
    f = drag_drop_window(&p, &drag, &i);
    i.ZOOMFACTOR = 100;
    window_handling_cal(&p, &i, f);
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
    //free(f->str);
    return (0);
}
