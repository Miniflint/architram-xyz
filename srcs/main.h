#ifndef MAIN_H
# define MAIN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

enum {
    WIDTH = 1000,
    HEIGHT = 800
};

/* utils */
int mySqrt(int x);
int get_strlen_delim(char *str, char delim);

/* image utils */
SDL_Texture* load_image(SDL_Renderer* renderer, const char* path);
void place_image(SDL_Renderer* renderer, SDL_Texture* imageTexture, int cur_spacing);

#endif