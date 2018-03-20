/*
** ex_select.c for ex_select in /home/antoin_g/rendu/Unix/test
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Tue May 23 18:27:11 2017 Loïs Antoine
** Last update Sun Jun 11 18:36:26 2017 Loïs Antoine
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "irc.h"

int			add_client(t_env *e, int s)
{
  int			cs;
  struct sockaddr_in	client_sin;
  unsigned int		client_sin_len;

  client_sin_len = sizeof(client_sin);
  if ((cs = accept(s, (struct sockaddr *)&client_sin, &client_sin_len)) == -1)
    return (1);
  e->fd_type[cs] = FD_CLIENT;
  e->fct[cs] = client_read;
  e->chan_location[cs] = NULL;
  e->iden[cs].nick = NULL;
  e->iden[cs].host = NULL;
  e->iden[cs].serv = NULL;
  e->iden[cs].real = NULL;
  e->iden[cs].user = NULL;
  e->iden[cs].regis = 0;
  e->iden[cs].ping_s = 0;
  e->iden[cs].ping_r = 0;
  e->buff_list[cs][0] = 0;
  return (0);
}

static int		add_server(t_env *e)
{
  int			s;
  struct sockaddr_in	sin;

  if ((s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    return (1);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(e->info.port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) == -1
      || listen(s, 42) == -1)
    return (1);
  e->fd_type[s] = FD_SERVER;
  e->fct[s] = server_read;
  time(&e->info.creating_date);
  return (0);
}

static int	get_max_fd(fd_set *fd_read, const t_env *e)
{
  int		i;
  int		fd_max;

  i = 0;
  while (i < MAX_FD)
    {
      if (e->fd_type[i] != FD_FREE)
	{
	  FD_SET(i, fd_read);
	  fd_max = i;
	}
      ++i;
    }
  return (fd_max + 1);
}

static int	action_all_fd(const t_env *e, fd_set *fd_read)
{
  int		i;
  int		check;

  i = 0;
  check = 0;
  while (i < MAX_FD)
    {
      if ((FD_ISSET(i, fd_read) || (e->fd_type[i] == FD_CLIENT
				    && e->buff_list[i][0])) && e->fct[i](e, i))
	return (1);
      if (e->fd_type[i] == FD_CLIENT && e->buff_list[i][0])
	check = 1;
      ++i;
      if (i == MAX_FD && check)
	{
	  i = 0;
	  check = 0;
	}
    }
  return (0);
}

int		main(int ac, const char **av)
{
  t_env		e;
  fd_set	fd_read;

  if (ac != 2 || !strcmp(av[1], "--help"))
    {
      printf("USAGE: ./server port\n");
      return (84);
    }
  srand(time(NULL));
  memset(e.fd_type, FD_FREE, MAX_FD);
  e.info.port = atoi(av[1]);
  if (add_server(&e))
    return (84);
  e.chan_list = NULL;
  while (1)
    {
      FD_ZERO(&fd_read);
      if (select(get_max_fd(&fd_read, &e), &fd_read, NULL, NULL, NULL) == -1
	  || action_all_fd(&e, &fd_read))
	return (84);
    }
  return (0);
}
