/*
** command_chan_server.c for command_chan_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Fri Jun  2 05:20:34 2017 Loïs Antoine
** Last update Sun Jun 11 18:13:55 2017 Loïs Antoine
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "irc.h"

static bool	tell_ping(t_env *e, int fd)
{
  int		i;

  if (e->iden[fd].regis && e->iden[fd].nick && !e->iden[fd].ping_s)
    {
      i = 0;
      while (i < 10)
	{
	  e->iden[fd].ping[i] = rand() % 92 + 33;
	  ++i;
	}
      e->iden[fd].ping[i] = 0;
      dprintf(fd, "PING :%s\n", e->iden[fd].ping);
      e->iden[fd].ping_s = 1;
    }
  return (0);
}

static bool	get_all(t_env *e, char **cmd, int i, int fd)
{
  int		size;
  int		idx;

  e->iden[fd].user = cmd[1];
  e->iden[fd].host = cmd[2];
  e->iden[fd].serv = cmd[3];
  idx = 3;
  size = 0;
  while (++idx < i)
    size = size + strlen(cmd[idx]) + 1;
  if (!(e->iden[fd].real = malloc(sizeof(char) * size)))
    return (1);
  e->iden[fd].real[0] = 0;
  idx = 3;
  while (++idx < i)
    {
      strcat(e->iden[fd].real, ((idx == 4 && cmd[idx][0] == ':')
				? cmd[idx] + 1 : cmd[idx]));
      free(cmd[idx]);
      strcat(e->iden[fd].real, ((idx < i - 1) ? " " : ""));
    }
  return (0);
}

bool	command_user(t_env *e, char *command, int fd)
{
  int	i;
  char	**cmd;

  if (e->iden[fd].regis)
    {
      dprintf(fd, "%s %s %s\n", E462_1, e->iden[fd].nick, E462_2);
      return (0);
    }
  if (!(cmd = my_str_to_wordtab(command)))
    return (1);
  i = 0;
  while (cmd[i])
    ++i;
  if (i < 5)
    {
      dprintf(fd, "%s USER %s\n", E461_1, E461_2);
      return (0);
    }
  if (get_all(e, cmd, i, fd))
    return (1);
  e->iden[fd].regis = 1;
  free(cmd[0]);
  free(cmd);
  return (tell_ping(e, fd));
}

static bool	is_nick(const char *nick, int fd)
{
  int		i;

  i = 0;
  while (nick[i])
    {
      if (nick[i] < '0' || (nick[i] > '9' && nick[i] < 'A')
	  || (nick[i] > 'Z' && nick[i] < 'a') || nick[i] > 'z')
	{
	  dprintf(fd, "%s %s %s\n", E432_1, nick, E432_2);
	  return (1);
	}
      ++i;
    }
  if (i > 9)
    {
      dprintf(fd, "%s %s %s\n", E432_1, nick, E432_2);
      return (1);
    }
  return (0);
}

bool	command_nick(t_env *e, char *command, int fd)
{
  int	i;

  if (strlen(command) < 6)
    dprintf(fd, "%s NICK %s\n", E461_1, E461_2);
  else
    {
      if (is_nick(command + 5, fd))
	return (0);
      i = 0;
      while (i < MAX_FD)
	{
	  if (fd != i && e->fd_type[i] == FD_CLIENT && e->iden[i].nick
	      && !strcmp(command + 5, e->iden[i].nick))
	    {
	      dprintf(fd, "%s %s %s\n", E433_1, command + 5, E433_2);
	      return (0);
	    }
	  ++i;
	}
      free(e->iden[fd].nick);
      if (!(e->iden[fd].nick = strdup(command + 5)))
	return (1);
    }
  return (tell_ping(e, fd));
}
