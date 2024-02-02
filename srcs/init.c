#include "main.h"
#include "main_struct.h"
#include "parse_file.h"
#include <string.h>


t_pos_triple **__init_array(char *str, long size)
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

    if (!path)
        return (NULL);
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
    file->pos = __init_array(file->trimmed_str, file->n_pts);
    if (!file->pos)
        return (NULL);
    return (file);
}
