#ifndef MAIN_H
# define MAIN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

const int WIDTH = 1000;
const int HEIGHT = 800;

/* utils */
int mySqrt(int x);
int get_strlen_delim(char *str, char delim);
char **ft_split(char const *s, char c);

#endif