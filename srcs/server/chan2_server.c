/*
** chan2_server.c for chan2_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Sat Jun 10 21:45:09 2017 Loïs Antoine
** Last update Sun Jun 11 18:49:35 2017 Loïs Antoine
*/

#include <string.h>
#include <stdio.h>
#include "irc.h"

static bool	chan_valid(const char *chan)
{
  int		i;

  i = 1;
  if (chan[0] != '#')
    return (1);
  while (chan[i] && chan[i] != ' ' && chan[i] != '#')
    ++i;
  return ((chan[i]) ? 1 : 0);
}

static bool	add_client_chan(t_env *e, const char *chan, int fd)
{
  int		i;

  if (!chan_exist(e->chan_location[fd], chan))
    {
      if (!(e->chan_location[fd] = add_channel(e->chan_location[fd], chan)))
	return (1);
      dprintf(fd, "%s %s = %s :", M353, e->iden[fd].nick, chan);
      i = 0;
      while (i < MAX_FD)
	{
	  if (e->fd_type[i] == FD_CLIENT
	      && chan_exist(e->chan_location[i], chan))
	    {
	      dprintf(fd, "%s ", e->iden[i].nick);
	      if (i != fd)
		dprintf(i, "%s JOIN %s\n", e->iden[fd].nick, chan);
	    }
	  ++i;
	}
      dprintf(fd, "\n");
      dprintf(fd, "%s %s %s %s\n", M366_1, e->iden[fd].nick, chan, M366_2);
    }
  return (0);
}

bool	command_join(t_env *e, char *command, int fd)
{
  if (!e->iden[fd].ping_r)
    dprintf(fd, "%s JOIN %s\n", E451_1, E451_2);
  else if (strlen(command) < 6)
    dprintf(fd, "%s JOIN %s\n", E461_1, E461_2);
  else
    {
      if (chan_valid(command + 5))
	dprintf(fd, "%s %s %s\n", E403_1, command + 5, E403_2);
      else if (chan_exist(e->chan_list, command + 5)
	       && add_client_chan(e, command + 5, fd))
	return (1);
      else if (!chan_exist(e->chan_list, command + 5))
	if (!(e->chan_list = add_channel(e->chan_list, command + 5))
	    || add_client_chan(e, command + 5, fd))
	  return (1);
    }
  return (0);
}

static void	leave_chan(t_env *e, const char *chan, int fd)
{
  int		i;
  int		check;

  e->chan_location[fd] = free_chan(e->chan_location[fd], chan);
  i = 0;
  check = 0;
  while (i < MAX_FD)
    {
      if (i != fd && e->fd_type[i] == FD_CLIENT
	  && chan_exist(e->chan_location[i], chan))
	{
	  dprintf(i, "%s PART %s\n", e->iden[fd].nick, chan);
	  check = 1;
	}
      ++i;
    }
  if (!check)
    e->chan_list = free_chan(e->chan_list, chan);
}

bool	command_part(t_env *e, char *command, int fd)
{
  if (!e->iden[fd].ping_r)
    dprintf(fd, "%s PART %s\n", E451_1, E451_2);
  else if (strlen(command) < 6)
    dprintf(fd, "%s PART %s\n", E461_1, E461_2);
  else
    {
      if (!chan_exist(e->chan_location[fd], command + 5))
	dprintf(fd, "%s %s %s %s\n", E403_1, e->iden[fd].nick,
		command + 5, E403_2);
      else
	leave_chan(e, command + 5, fd);
    }
  return (0);
}
