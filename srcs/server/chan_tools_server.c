/*
** chan_tools_server.c for chan_tools_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Sun Jun 11 15:12:32 2017 Loïs Antoine
** Last update Sun Jun 11 18:50:53 2017 Loïs Antoine
*/

#include <string.h>
#include <stdio.h>
#include "irc.h"

static void	write_names(const t_env *e, const char *chan, int fd)
{
  int		i;

  dprintf(fd, "%s %s = %s :", M353, e->iden[fd].nick, chan);
  i = 0;
  while (i < MAX_FD)
    {
      if (e->fd_type[i] == FD_CLIENT
	  && chan_exist(e->chan_location[i], chan))
	dprintf(fd, "%s ", e->iden[i].nick);
      ++i;
    }
  dprintf(fd, "\n");
  dprintf(fd, "%s %s %s %s\n", M366_1, e->iden[fd].nick, chan, M366_2);
}

bool		command_name(t_env *e, char *command, int fd)
{
  if (!e->iden[fd].ping_r)
    dprintf(fd, "%s NAMES %s\n", E451_1, E451_2);
  else if (strlen(command) < 7)
    dprintf(fd, "%s NAMES %s\n", E461_1, E461_2);
  else
    write_names(e, command + 6, fd);
  return (0);
}

bool		command_list(t_env *e, char *command, int fd)
{
  t_chan	*tmp;

  if (!e->iden[fd].ping_r)
    dprintf(fd, "%s PART %s\n", E451_1, E451_2);
  else
    {
      dprintf(fd, "%s %s %s\n", M321_1, e->iden[fd].nick, M321_2);
      tmp = e->chan_list;
      while (tmp)
	{
	  if (strlen(command) < 6 || !strcmp(tmp->name, command + 5))
	    dprintf(fd, "%s %s %s %d\n", M322, e->iden[fd].nick,
		    tmp->name, chan_users(e, tmp->name));
	  tmp = tmp->next;
	}
      dprintf(fd, "%s %s %s\n", M323_1, e->iden[fd].nick, M323_2);
    }
  return (0);
}
