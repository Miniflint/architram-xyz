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
