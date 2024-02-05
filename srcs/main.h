#ifndef MAIN_H
# define MAIN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#define DISPLAY_FULLPATH_FILE 50
#define DISPLAY_FILE_NAME 100
#define DISPLAY_LOADED 150

enum {
    WIDTH = 1000,
    HEIGHT = 800
};

/* utils */
int     mySqrt(int x);
char    *get_bck_slash(char *str);
int     custom_strstr(char *str1, char *str2);
int     get_strlen_delim(char *str, char delim);
void    free_double_array(void **arr, int size);

/* image utils */
SDL_Texture* load_image(SDL_Renderer* renderer, const char* path);
void draw_halfs(SDL_Renderer *renderer, const int bck[4], const SDL_Rect half, char *str);
void displayText(SDL_Renderer* renderer, char *text, int x, int y, int size_font, const SDL_Color textColor);
void displayTextOnRect(SDL_Renderer* renderer, char *text, SDL_Rect destRect, int size_font, const SDL_Color textColor);


#endif