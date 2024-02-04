#include "main.h"
#include "main_struct.h"
#include "parse_file.h"

#define DISPLAY_FULLPATH_FILE 50
#define DISPLAY_FILE_NAME 100
#define DISPLAY_LOADED 150

void displayTextOnRect(SDL_Renderer* renderer, char *text, SDL_Rect destRect, int size_font, const SDL_Color textColor)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;

    font = TTF_OpenFont("ARIAL.TTF", size_font);
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destRect.w = surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void displayText(SDL_Renderer* renderer, char *text, int x, int y, int size_font, const SDL_Color textColor)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect destRect;

    font = TTF_OpenFont("ARIAL.TTF", size_font);
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destRect.x = x;
    destRect.y = y;
    destRect.w = (surface->w > WIDTH) ? WIDTH - 1 : surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_rect(t_sdl_data *p, SDL_Rect *rect, t_pos pos, t_pos size, const int rgba[4], int fill)
{
    rect->x = pos.x;
    rect->y = pos.y;
    rect->w = size.x - pos.x;
    rect->h = size.y - pos.y;
    SDL_SetRenderDrawColor(p->renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderDrawRect(p->renderer, rect);
    if (fill)
        SDL_RenderFillRect(p->renderer, rect);
}

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
        draw_rect(p, &mouse->draw_rect, mouse->down_pos, mouse->current_pos, black_draw, 1);
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
    if (f)
        printf("hi\n");
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
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

void draw_halfs(SDL_Renderer *renderer, const int bck[4], const SDL_Rect half, char *str)
{
    SDL_SetRenderDrawColor(renderer, bck[0], bck[1], bck[2], bck[3]);
    SDL_RenderFillRect(renderer, &half);
    displayTextOnRect(renderer, str, half, 30, (const SDL_Color){255, 255, 255, 255});
}
// TODO: make displayed text disapear between "file loading" and "file loaded" / error
int thread_load_jpg(void *all)
{
    t_all *a;

    a = (t_all *)all;
    displayText(a->data->renderer, "[INFO]: File loading", 0, DISPLAY_LOADED, 25, (const SDL_Color){255, 255, 0, 255});
    a->image->imageTexture = load_image(a->data->renderer, a->drag->path_jpg);
    if (!a->image->imageTexture)
    {
        displayText(a->data->renderer, (char *)IMG_GetError(), 0, DISPLAY_LOADED, 25, (const SDL_Color){255, 0, 0, 255});
        return (1);
    }
    displayText(a->data->renderer, "[INFO]: File loaded", 0, DISPLAY_LOADED, 25, (const SDL_Color){0, 255, 0, 255});
    return (0);
}

int thread_load_xyz(void *all)
{
    t_all *a;

    a = (t_all *)all;
    a->file = __init_file(a->drag->path_xyz);
    displayText(a->data->renderer, "[INFO]: File loading", 0, DISPLAY_LOADED, 25, (const SDL_Color){255, 255, 0, 255});
    if (!a->file)
    {
        displayText(a->data->renderer, "Unsupported file format", 0, (HEIGHT / 2) + DISPLAY_LOADED, 25, (const SDL_Color){255, 0, 0, 255});
        return (1);
    }
    displayText(a->data->renderer, "[INFO]: File loaded", 0, (HEIGHT / 2) + DISPLAY_LOADED, 25, (const SDL_Color){0, 255, 0, 255});
    return (0);
}

void drag_drop_window(t_sdl_data *p, t_drag *drag, t_sdl_image *i)
{
    t_all all;
    const SDL_Rect top_half = {
        0, 0, 
        WIDTH,  HEIGHT / 2
    };
    const SDL_Rect bot_half = {
        0, (HEIGHT / 2),
        WIDTH, HEIGHT / 2
    };
    const int jpg[4] = {0, 0, 0, 255};
    const int xyz[4] = {0, 0, 0, 255};

    all.data = p;
    all.drag = drag;
    all.image = i;
    if (!all.image)
        return ;
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
    draw_halfs(p->renderer, jpg, top_half, "Drop your JPG here");
    draw_halfs(p->renderer, xyz, bot_half, "Drop your XYZ here");
    while (1)
    {
        if (SDL_PollEvent(&p->windowEvent))
        {
            if (p->windowEvent.type == SDL_QUIT)
                break ;
            else if (p->windowEvent.type == SDL_KEYDOWN)
            {
                if (p->windowEvent.key.keysym.sym == SDLK_ESCAPE)
                    break ;
            }
            if (p->windowEvent.type == SDL_DROPFILE)
            {
                if (
                    custom_strstr(p->windowEvent.drop.file, ".jpg") == 0 ||
                    custom_strstr(p->windowEvent.drop.file, ".png") == 0 ||
                    custom_strstr(p->windowEvent.drop.file, ".bmp") == 0 ||
                    custom_strstr(p->windowEvent.drop.file, ".pnm") == 0 ||
                    custom_strstr(p->windowEvent.drop.file, ".xpm") == 0
                )
                {
                    if (all.drag->path_jpg)
                        SDL_free(drag->path_jpg);
                    drag->path_jpg = p->windowEvent.drop.file;
                    draw_halfs(p->renderer, jpg, top_half, "Drop your JPG here");
                    displayText(p->renderer, drag->path_jpg, 0, DISPLAY_FULLPATH_FILE, 25, (const SDL_Color){255, 255, 255, 255});
                    displayText(p->renderer, get_bck_slash(drag->path_jpg), 0, DISPLAY_FILE_NAME, 25, (const SDL_Color){255, 255, 255, 255});
                    SDL_CreateThread(thread_load_jpg, "random", &all);
                }
                else
                {
                    if (all.drag->path_xyz)
                        SDL_free(drag->path_xyz);
                    drag->path_xyz = p->windowEvent.drop.file;
                    draw_halfs(p->renderer, xyz, bot_half, "Drop your XYZ here");
                    displayText(p->renderer, drag->path_xyz, 0, HEIGHT / 2 + DISPLAY_FULLPATH_FILE, 25, (const SDL_Color){255, 255, 255, 255});
                    displayText(p->renderer, get_bck_slash(drag->path_xyz), 0, HEIGHT / 2 + DISPLAY_FILE_NAME, 25, (const SDL_Color){255, 255, 255, 255});
                    SDL_CreateThread(thread_load_xyz, "random2", &all);
                }
            }
        }
    }
}

int SDL_main(int argc, char **argv)
{
    t_sdl_data  p;
    t_sdl_image i;
    t_file *f;
    t_drag drag;

    if (argc < 2)
    {
        fprintf(stderr, "Provide a .jpg file please");
        return (1);
    }
    i.cur_offset.x = 0;
    i.cur_offset.y = 0;
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
    drag_drop_window(&p, &drag, &i);
    return (0);
    displayText(p.renderer, "LOADING FILE", 0, 0, 50, (const SDL_Color){255, 255, 255, 255});
	i.imageTexture = load_image(p.renderer, argv[1]);
	if (!i.imageTexture)
		return (1);
    i.ZOOMFACTOR = 100;
    f = __init_file(NULL);
    //if (!f)
    //    return (1);
    window_handling_cal(&p, &i, f);
    SDL_Quit();
    //free(f->str);
    return (0);
}
