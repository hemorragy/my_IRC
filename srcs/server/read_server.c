/*
** read_server.c for read_server in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Wed May 24 18:25:42 2017 Loïs Antoine
** Last update Sun Jun 11 18:09:50 2017 Loïs Antoine
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "irc.h"

static const t_comm	g_comm[] =
  {
    {"NICK", &command_nick},
    {"USER", &command_user},
    {"QUIT", &command_quit},
    {"PONG", &command_pong},
    {"JOIN", &command_join},
    {"PART", &command_part},
    {"LIST", &command_list},
    {"NAMES", &command_name},
    {"PRIVMSG", &command_pmsg},
    {NULL, NULL}
  };

static int	get_end_comm(const char *command)
{
  int		i;

  i = 0;
  while (command[i] && command[i] != ' ')
    ++i;
  return (i);
}

static int	test_command(t_env *e, char *command, int fd)
{
  int		i;

  i = 0;
  while (g_comm[i].command)
    {
      if (!strncmp(command, g_comm[i].command, get_end_comm(command)))
	{
	  if (g_comm[i].func(e, command, fd))
	    return (-1);
	  return (1);
	}
      ++i;
    }
  return (0);
}

static int	read_ok(t_env *e, int fd, char *command)
{
  int		check;

  if (!command)
    return (1);
  check = 0;
  if ((check = test_command(e, command, fd)) == -1)
    return (1);
  if (!check)
    dprintf(fd, "%s %s %s %s\n", E421_1, e->iden[fd].nick, command, E421_2);
  free(command);
  return (0);
}

int	client_read(t_env *e, int fd)
{
  char	*buf;

  buf = get_next_line(fd, e);
  if (buf)
    {
      if (read_ok(e, fd, epure_str(buf)))
	return (84);
      free(buf);
    }
  else
    command_quit(e, NULL, fd);
  return (0);
}

int	server_read(t_env *e, int fd)
{
  printf("New client\n");
  return (add_client(e, fd));
}
