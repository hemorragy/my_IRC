/*
** identify2_server.c for identify2_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Sat Jun 10 18:51:33 2017 Loïs Antoine
** Last update Sun Jun 11 18:32:46 2017 Loïs Antoine
*/

#include <string.h>
#include <stdio.h>
#include "irc.h"

static void	tell_connect(const t_env *e, int fd)
{
  dprintf(fd, "%s %s %s %s!%s@%s\n", M001_1, e->iden[fd].nick, M001_2,
	  e->iden[fd].nick, e->iden[fd].user, e->iden[fd].host);
  dprintf(fd, "%s %s %s\n", M002_1, e->iden[fd].nick, M002_2);
  dprintf(fd, "%s %s %s %s", M003_1, e->iden[fd].nick, M003_2,
	  ctime(&e->info.creating_date));
}

bool	command_pong(t_env *e, char *command, int fd)
{
  if (!e->iden[fd].ping_s)
    dprintf(fd, "%s PONG %s\n", E451_1, E451_2);
  else if ((strlen(command) == 15 && !strcmp(command + 5, e->iden[fd].ping))
	   || (strlen(command) == 16 && command[5] == ':'
	       && !strcmp(command + 6, e->iden[fd].ping)))
    {
      e->iden[fd].ping_r = 1;
      tell_connect(e, fd);
    }
  return (0);
}
