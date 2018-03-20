/*
** message_server.c for message_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Sun Jun 11 07:04:19 2017 Loïs Antoine
** Last update Sun Jun 11 18:12:35 2017 Loïs Antoine
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "irc.h"

static void	send_all(const t_env *e, char **command, int i, int fd)
{
  t_chan	*tmp;

  if (e->fd_type[i] == FD_CLIENT)
    {
      tmp = e->chan_location[i];
      while (tmp)
	{
	  if (!strcmp(tmp->name, command[1]))
	    dprintf(i, "%s PRIVMSG %s :%s\n", e->iden[fd].nick,
		    tmp->name, command[2]);
	  tmp = tmp->next;
	}
    }
}

static void	send_to_chan(const t_env *e, char **command, int fd)
{
  t_chan	*tmp;
  int		check;
  int		i;

  i = 0;
  check = 0;
  tmp = e->chan_location[fd];
  while (tmp)
    {
      if (!strcmp(tmp->name, command[1]))
	check = 1;
      tmp = tmp->next;
    }
  if (check)
    while (i < MAX_FD)
      {
	send_all(e, command, i, fd);
	++i;
      }
  else
    dprintf(fd, "%s %s %s %s\n", E404_1, e->iden[fd].nick, command[1], E404_2);
}

static void	send_to_user(const t_env *e, char **command, int fd)
{
  int		i;
  int		check;

  i = 0;
  check = 0;
  while (i < MAX_FD)
    {
      if (e->fd_type[i] == FD_CLIENT && !strcmp(e->iden[i].nick, command[1]))
	{
	  dprintf(i, "%s PRIVMSG %s :%s\n", e->iden[fd].nick,
		  e->iden[i].nick, command[2]);
	  check = 1;
	}
      ++i;
    }
  if (!check)
    dprintf(fd, "%s %s %s %s\n", E401_1, e->iden[fd].nick, command[2], E401_2);
}

static char	**get_command(const char *command)
{
  char		**tab;
  int		size;
  int		i;

  if (!(tab = my_str_to_wordtab(command)))
    return (NULL);
  i = 1;
  size = 0;
  if (!tab[0] || !tab[1] || !tab[2])
    return (tab);
  while (tab[++i])
    size = strlen(tab[i]) + size + 1;
  if (!(tab[2] = realloc(tab[2], sizeof(char) * size)))
    return (NULL);
  i = 2;
  while (tab[++i])
    {
      strcat(tab[2], tab[i]);
      free(tab[i]);
      strcat(tab[2], ((tab[i + 1]) ? " " : ""));
    }
  tab[3] = NULL;
  return (tab);
}

bool	command_pmsg(t_env *e, char *command, int fd)
{
  char	**tab;

  if (!e->iden[fd].ping_r)
    dprintf(fd, "%s PRIVMSG %s\n", E451_1, E451_2);
  else
    {
      if (!(tab = get_command(command)))
	return (1);
      if (!tab[0] || !tab[1] || !tab[2])
	dprintf(fd, "%s PRIVMSG %s\n", E461_1, E461_2);
      else
	{
	  if (tab[1][0] == '#')
	    send_to_chan(e, tab, fd);
	  else
	    send_to_user(e, tab, fd);
	}
    }
  return (0);
}
