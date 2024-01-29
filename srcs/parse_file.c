#include "parse_file.h"

static long get_file_size(FILE *f)
{
    long size;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return (size);
}

int check_header(char *str)
{
    if (str[0] != 'X')
        return (1);
    if (str[1] != ' ')
        return (1);
    if (str[2] != 'Y')
        return (1);
    if (str[3] != ' ')
        return (1);
    if (str[4] != 'Z')
        return (1);
    if (str[5] != '\n')
        return (1);
    return (0);
}

char *read_file(char *path, long int *size)
{
    char *str;
    FILE *f;

    f = fopen(path, "r");
    if (!f)
    {
        fprintf(stderr, "Could not open the file to read");
        return (NULL);
    }
    *size = get_file_size(f);
    if (*size == -1)
        return (NULL);
    str = (char *)malloc(sizeof(char) * (*size + 1));
    if (!str)
        return (NULL);
    fread(str, *size, 1, f);
    fclose(f);
    if (check_header(str))
        return (NULL);
    return (str);
}
