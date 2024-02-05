#include <stdlib.h>

int mySqrt(int x)
{
    long  i;

    if (x < 4)
      return (!x ? x : 1);
    i = x / (x / 2);
    while (i * i < x)
      i++;
    return (i * i == x ? i : -1);
}

int get_strlen_delim(char *str, char delim)
{
    int i;
    i = 0;
    while (str[i] && str[i] != delim)
        i++;
    return (i);
}

char to_lower(char c)
{
    return (c >= 'A' && c <= 'Z' ? c - 32 : c);
}

int custom_strstr(char *str1, char *str2)
{
    int i;
    int j;

    i = 0;
    if (!str1 || !str2)
        return (-1);
    while (str1[i])
    {
        if (to_lower(str1[i]) == str2[0])
        {
            j = 0;
            while (str1[i + j] && str2[j] && to_lower(str1[i + j]) == str2[j])
                j++;
            if (!str2[j])
                return (0);
        }
        i++;
    }
    return (1);
}

char *get_bck_slash(char *str)
{
    int i;
    int size;

    size = get_strlen_delim(str, '\0');
    i = size;
    while (i > 0)
    {
        if (str[i] == '/' || str[i] == '\\')
            return (&str[i] + 1);
        i--;
    }
    return (str);
}

void free_double_array(void **arr, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        if (arr[i])
        {
            free(arr[i]);
            arr[i] = (void *)0;
        }
        i++;
    }
}