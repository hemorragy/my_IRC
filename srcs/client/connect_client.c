/*
** connect_client.c for connect_client in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Wed May 31 01:03:52 2017 Loïs Antoine
** Last update Sun Jun 11 18:58:40 2017 Loïs Antoine
*/

#include <string.h>
#include "irc.h"

static void	get_string_ip(t_client *client)
{
  int		i;
  int		id;
  int		idx;

  i = 0;
  id = 0;
  while (id < 4)
    {
      idx = 0;
      while (client->ip_serv[id][idx])
	{
	  client->ip_serv_string[i] = client->ip_serv[id][idx];
	  ++i;
	  ++idx;
	}
      ++id;
      if (id == 4)
	client->ip_serv_string[i] = 0;
      else
	client->ip_serv_string[i] = '.';
      ++i;
    }
}

static int	take_ip(t_client *client, const char *buff, int *start, int nb)
{
  int		i;

  i = 0;
  while (buff[*start] && buff[*start] != ':' && buff[*start] != '.')
    {
      if (buff[*start] >= '0' && buff[*start] <= '9')
	client->ip_serv[nb][i] = buff[*start];
      else
	return (1);
      ++i;
      ++*start;
    }
  if ((nb != 3 && buff[*start] != '.')
      || (nb == 3 && buff[*start] != ':' && buff[*start]))
    return (1);
  client->ip_serv[nb][i] = 0;
  *start = (buff[*start] == ':' || !buff[*start]) ? *start : *start + 1;
  return (0);
}

static int	take_port(t_client *client, const char *buff, int *i)
{
  int		idx;

  idx = 0;
  while (buff[*i] && idx < 5)
    {
      if (buff[*i] >= '0' && buff[*i] <= '9')
	client->port_serv[idx] = buff[*i];
      else
	return (1);
      ++*i;
      ++idx;
    }
  if (idx == 5)
    return (1);
  client->port_serv[idx] = 0;
  return (0);
}

int	get_connect(t_client *client, const char *buff)
{
  int	i;

  if (!strncmp(buff, "/server ", 8) && strlen(buff) >= 15)
    {
      i = 8;
      if (take_ip(client, buff, &i, 0) || take_ip(client, buff, &i, 1)
	  || take_ip(client, buff, &i, 2) || take_ip(client, buff, &i, 3))
	return (1);
      ++i;
      if (buff[i - 1] == ':')
	{
	  if (take_port(client, buff, &i))
	    return (1);
	}
      else if (!buff[i - 1])
	strcpy(client->port_serv, "6667");
      else
	return (1);
      get_string_ip(client);
      return (0);
    }
  return (1);
}
