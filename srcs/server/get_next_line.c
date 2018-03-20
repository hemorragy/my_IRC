/*
** get_next_line.c for get_next_line in /home/antoin_g/rendu/C_prog_elem/CPE_2015_getnextline
**
** Made by Loïs Antoine
** Login   <antoin_g@epitech.net>
**
** Started on  Mon Jan  4 14:54:30 2016 Loïs Antoine
** Last update Sat Jun 10 18:31:02 2017 Loïs Antoine
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "irc.h"

static int	muli(const char *str, int choice)
{
  int		i;

  i = 0;
  if (choice == 0)
    i = strlen(str);
  else if (choice == 1)
    while (str[i] != '\n' && str[i] != 0)
      ++i;
  else
    i = (muli(str, 0) != muli(str, 1));
  return (i);
}

static int	stack_line(char *buffer, t_gnl *gnl, int choice)
{
  if (gnl->line == NULL)
    {
      if (!(gnl->line = strndup(buffer, muli(buffer, choice))))
	return (1);
    }
  else
    {
      if (!(gnl->line = realloc(gnl->line, sizeof(char)
				* (muli(buffer, choice)
				   + 2 + muli(gnl->line, 0)))))
	return (1);
      strncat(gnl->line, buffer, muli(buffer, choice));
    }
  if (choice == 1)
    {
      memmove(buffer, buffer + muli(buffer, 1) + 1,
	      strlen(buffer + muli(buffer, 1) + 1) + 1);
      gnl->test = 1;
    }
  else
    buffer[0] = 0;
  return (0);
}

static int	gnl_next(char *buffer, t_gnl *gnl, int fd)
{
  int		size;

  if (buffer[0] == 0)
    {
      if (!(size = read(fd, buffer, READ_SIZE)))
	{
	  if (gnl->line == NULL)
	    return (1);
	  else
	    gnl->test = 1;
	}
      if (size == -1)
	return (1);
      buffer[size] = 0;
    }
  return (0);
}

char		*get_next_line(int fd, t_env *e)
{
  t_gnl		gnl;

  gnl.test = 0;
  gnl.line = NULL;
  while (gnl.test == 0)
    if (gnl_next(e->buff_list[fd], &gnl, fd) ||
	stack_line(e->buff_list[fd], &gnl, muli(e->buff_list[fd], 2)))
      return (NULL);
  return (gnl.line);
}
