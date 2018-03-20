/*
** chan_server.c for chan_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Fri Jun  2 01:29:28 2017 Loïs Antoine
** Last update Sun Jun 11 18:20:03 2017 Loïs Antoine
*/

#include <string.h>
#include <stdlib.h>
#include "irc.h"

int		chan_users(const t_env *e, const char *chan)
{
  t_chan	*tmp;
  int		count;
  int		i;

  i = 0;
  count = 0;
  while (i < MAX_FD)
    {
      if (e->fd_type[i] == FD_CLIENT)
	{
	  tmp = e->chan_location[i];
	  while (tmp)
	    {
	      if (!strcmp(tmp->name, chan))
		++count;
	      tmp = tmp->next;
	    }
	}
      ++i;
    }
  return (count);
}

t_chan		*free_chan(t_chan *chan, const char *name)
{
  t_chan	*tmp;
  t_chan	*prev;

  tmp = chan;
  prev = NULL;
  while (tmp)
    {
      if (!strcmp(tmp->name, name))
	{
	  free(tmp->name);
	  if (prev)
	    prev->next = tmp->next;
	  if (!prev)
	    prev = tmp->next;
	  free(tmp);
	  return (prev);
	}
      prev = tmp;
      tmp = tmp->next;
    }
  return (chan);
}

void		free_all_chan(t_chan *chan)
{
  t_chan	*tmp;
  t_chan	*prev;

  tmp = chan;
  while (tmp)
    {
      prev = tmp;
      free(tmp->name);
      tmp = tmp->next;
      free(prev);
    }
}

bool		chan_exist(t_chan *chan, const char *name)
{
  t_chan	*tmp;

  tmp = chan;
  while (tmp)
    {
      if (!strcmp(tmp->name, name))
	return (1);
      tmp = tmp->next;
    }
  return (0);
}

t_chan		*add_channel(t_chan *chan, const char *name)
{
  t_chan	*new;

  if (!(new = malloc(sizeof(t_chan))) || !(new->name = strdup(name)))
    return (NULL);
  new->next = chan;
  new->nb = 0;
  return (new);
}
