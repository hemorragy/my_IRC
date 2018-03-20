/*
** utils.c for utils in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Tue Jun  6 19:18:18 2017 Loïs Antoine
** Last update Sat Jun 10 03:14:56 2017 Loïs Antoine
*/

#include <stdlib.h>
#include <string.h>

static int	count_word(const char *str)
{
  int		i;
  int		count;

  i = 0;
  count = 0;
  while (str[i])
    {
      if (i == 0 || str[i - 1] == ' ')
	++count;
      ++i;
    }
  return (count);
}

char	**my_str_to_wordtab(const char *str)
{
  char	**tab;
  int	prev;
  int	idx;
  int	i;

  if (!(tab = malloc(sizeof(char *) * (count_word(str) + 1))))
    return (NULL);
  i = 0;
  idx = 0;
  while (str[i])
    {
      prev = i;
      while (str[i] && str[i] != ' ')
	++i;
      if (!(tab[idx] = strndup(str + prev, i - prev)))
	return (NULL);
      ++idx;
      i = (str[i]) ? i + 1 : i;
    }
  tab[idx] = NULL;
  return (tab);
}

static void	epure_tools(const char *str, char *new, int *i, int *idx)
{
  if (*i)
    {
      new[*idx] = ' ';
      ++*idx;
    }
  while (str[*i] > '\0' && str[*i] <= ' ')
    ++*i;
}

char	*epure_str(char *str)
{
  char	*new;
  int	i;
  int	idx;

  i = 0;
  idx = 0;
  if (!(new = malloc(sizeof(char) * (strlen(str) + 1))))
    return (NULL);
  while (str[i])
    {
      if (str[i] > '\0' && str[i] <= ' ')
	epure_tools(str, new, &i, &idx);
      else
	{
	  new[idx] = str[i];
	  ++idx;
	  ++i;
	}
    }
  if (idx > 0 && new[idx - 1] == ' ')
    --idx;
  new[idx] = 0;
  return (new);
}
