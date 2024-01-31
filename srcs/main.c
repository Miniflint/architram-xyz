#include "main.h"
#include "main_struct.h"
#include "parse_file.h"
#include <string.h>


void displayText(SDL_Renderer* renderer, char *text, int x, int y) {

    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect destRect;
    const SDL_Color textColor
        = {255, 255, 255};

    font = TTF_OpenFont("ARIAL.TTF", 50);
    surface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destRect.x = x;
    destRect.y = y;
    destRect.w = surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_rect(t_sdl_data *p, SDL_Rect *rect, t_pos pos, t_pos size, const int rgba[4])
{
    rect->x = pos.x;
    rect->y = pos.y;
    rect->w = size.x - pos.x;
    rect->h = size.y - pos.y;
    SDL_SetRenderDrawColor(p->renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    SDL_RenderDrawRect(p->renderer, rect);
    SDL_RenderFillRect(p->renderer, rect);
}

SDL_Point *create_pts(int size, const int spacing)
{
    int i;
    int j;
    int index;
    t_pos *base_coordinate;
    SDL_Point *pts;
    const int full_size = 
        size * size;

    i = 0;
    index = 0;
    base_coordinate = current_pos(NULL);
    pts = (SDL_Point *)malloc(sizeof(SDL_Point) * full_size);
    while (i < size)
    {
        j = 0;
        while (j < size)
        {
            pts[index].x = i * spacing + base_coordinate->x;
            pts[index].y = j * spacing + base_coordinate->y;
            index++;
            j++;
        }
        i++;
    }
    return (pts);
}

void renderCoordinates(SDL_Renderer* renderer, int size, const int spacing)
{
    SDL_Point* points;

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    points = create_pts(size, spacing);
    SDL_RenderDrawPoints(renderer, points, size * size);
    free(points);
}

void calculate_mouse_offset(int *base_offsetX, int *base_offsetY, int new_posX, int new_posY)
{
    *base_offsetX = *base_offsetX - new_posX;
    *base_offsetY = *base_offsetY - new_posY;
}

void window_handling(t_sdl_data *p, t_sdl_image *i, t_file *f)
{
    int             track_mouse;
    t_track_mouse   mouse;
    const int white_draw[4] = 
        {255, 255, 255, 100};
    const int black_draw[4] = 
        {0, 0, 0, 100};

    track_mouse = 0;
    mouse.current_pos.x = 0;
    mouse.current_pos.y = 0;
    i->cur_spacing = 0;
    SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    SDL_RenderClear(p->renderer);
    current_pos(&(i->cur_offset));
    place_image(p->renderer, i->imageTexture, i->cur_spacing);
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
                if (p->windowEvent.key.keysym.sym == SDLK_r)
                {
                    i->cur_offset.x = 0;
                    i->cur_offset.y = 0;
                    i->cur_spacing = 0;
                }
            }
            else if (p->windowEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
                {
                    track_mouse = 1;
                    mouse.down_pos.x = p->windowEvent.button.x;
                    mouse.down_pos.y = p->windowEvent.button.y;
                }
                else if (p->windowEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    track_mouse = 2;
                    mouse.before_offset.x = p->windowEvent.button.x;
                    mouse.before_offset.y = p->windowEvent.button.y;
                }
            }
            else if (p->windowEvent.type == SDL_MOUSEBUTTONUP)
            {
                if (p->windowEvent.button.button == SDL_BUTTON_LEFT)
                {
                    track_mouse = 0;
                    mouse.up_pos.x = p->windowEvent.button.x;
                    mouse.up_pos.y = p->windowEvent.button.y;
    				SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    				SDL_RenderClear(p->renderer);
                }
                else if (p->windowEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    track_mouse = 0;
                    mouse.after_offset.x = p->windowEvent.button.x;
                    mouse.after_offset.y = p->windowEvent.button.y;
                }
            }
            else if (p->windowEvent.type == SDL_MOUSEWHEEL)
            {
                SDL_GetMouseState(&mouse.current_pos.x, &mouse.current_pos.y);
                i->cur_spacing += p->windowEvent.wheel.y * 100;
                i->cur_offset.x -= ((float)(mouse.current_pos.x) / WIDTH) * (100 * p->windowEvent.wheel.y);
                i->cur_offset.y -= ((float)(mouse.current_pos.y) / WIDTH) * (100 * p->windowEvent.wheel.y);
            }
    		SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
    		SDL_RenderClear(p->renderer);
            place_image(p->renderer, i->imageTexture, i->cur_spacing);
            if (track_mouse == 1)
            {
                mouse.last_pos.x = mouse.current_pos.x;
                mouse.last_pos.y = mouse.current_pos.y;
                SDL_GetMouseState(&mouse.current_pos.x, &mouse.current_pos.y);
                //if (mouse.last_pos.x)
                //    draw_rect(p, &mouse.draw_rect_delete, mouse.down_pos, mouse.last_pos, black_draw);
                draw_rect(p, &mouse.draw_rect, mouse.down_pos, mouse.current_pos, black_draw);
            }
            else if (track_mouse == 2)
            {
                mouse.after_offset.x = p->windowEvent.button.x;
                mouse.after_offset.y = p->windowEvent.button.y;
                i->cur_offset.x += mouse.after_offset.x - mouse.before_offset.x;
                i->cur_offset.y += mouse.after_offset.y - mouse.before_offset.y;
                mouse.before_offset = mouse.after_offset;
            }
            SDL_RenderPresent(p->renderer);
        }
    }
	SDL_DestroyTexture(i->imageTexture);
	SDL_DestroyRenderer(p->renderer);
    SDL_DestroyWindow(p->window);
}

t_pos_triple **__init_array(char *str, long size, int bloc_len)
{
    long i;
    int y;
    char *token;
    t_pos_triple **new;

    new = (t_pos_triple **)malloc(sizeof(t_pos_triple *) * size);
    if (!new)
        return (NULL);
    i = 0;
    while (i < size)
    {
        new[i] = (t_pos_triple *)malloc(sizeof(t_pos_triple) * size);
        if (!new[i])
        {
            while (--i > 0)
                free(new[i]);
            return (NULL);
        }
        i++;
    }
    i = 0;
    token = strtok(str, " \n");
    while(i < size)
    {
        y = 0;
        while (y < size)
        {
            new[i][y].x = round(atof(token) * 100) / 100;
            token = strtok(NULL, " \n");
            new[i][y].y = round(atof(token) * 100) / 100;
            token = strtok(NULL, " \n");
            new[i][y].z = round(atof(token) * 100) / 100;
            token = strtok(NULL, " \n");
            y++;
        }
        i++;
    }
    return (new);
}

t_file *__init_file(char *path)
{
    t_file      *file;

    file = (t_file *)malloc(sizeof(t_file) * 1);
    if (!file)
        return (NULL);
    file->str = read_file(path, &file->size);
    if (!file->str)
        return (NULL);
    file->trimmed_str = (file->str) + 6;
    file->real_size = file->size - 7;
    file->line_len = get_strlen_delim(file->trimmed_str, '\n') + 2;
    file->n_pts = mySqrt(file->real_size / file->line_len);
    if (file->n_pts < 1)
        return (NULL);
    file->pos = __init_array(file->trimmed_str, file->n_pts, file->line_len);
    if (!file->pos)
        return (NULL);
    return (file);
}

int main(int argc, char **argv)
{
    t_sdl_data  p;
    t_sdl_image i;
    t_file *f;

    if (argc < 2)
    {
        fprintf(stderr, "Provide a .jpg file please");
        return (1);
    }
    i.cur_offset.x = 0;
    i.cur_offset.y = 0;
    SDL_Init(SDL_INIT_EVENTS);
    TTF_Init();
    p.window = SDL_CreateWindow("XYZ visuals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!p.window)
    {
        fprintf(stderr, "Could not create window");
        return (1);
    }
    p.renderer = SDL_CreateRenderer(p.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(p.renderer, SDL_BLENDMODE_BLEND);
    displayText(p.renderer, "LOADING FILE", 0, 0);
	i.imageTexture = load_image(p.renderer, argv[1]);
	if (!i.imageTexture)
		return (1);
    //f = __init_file(argv[2]);
    //if (!f)
    //    return (1);
    window_handling(&p, &i, f);
    SDL_Quit();
    //free(f->str);
    return (0);
}
