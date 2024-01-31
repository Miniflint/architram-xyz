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
void printf_from_double_float_array(float **arr, int i);

#endif