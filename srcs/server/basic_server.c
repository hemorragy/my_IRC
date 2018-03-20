/*
** basic_server.c for basic_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Sat Jun 10 03:58:48 2017 Loïs Antoine
** Last update Sun Jun 11 18:29:38 2017 Loïs Antoine
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "irc.h"

bool	command_quit(t_env *e, char *command, int fd)
{
  (void)command;
  printf("%d: Connection closed\n", fd);
  close(fd);
  free_all_chan(e->chan_location[fd]);
  free(e->iden[fd].nick);
  free(e->iden[fd].user);
  free(e->iden[fd].host);
  free(e->iden[fd].serv);
  free(e->iden[fd].real);
  e->buff_list[fd][0] = 0;
  e->fd_type[fd] = FD_FREE;
  return (0);
}
