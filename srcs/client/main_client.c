/*
** main_client.c for main_client in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Tue May 30 17:14:54 2017 Loïs Antoine
** Last update Sun Jun 11 18:56:34 2017 Loïs Antoine
*/

#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "irc.h"

static int		connecting(t_client *client)
{
  struct sockaddr_in	addr_serv;
  int			fd_serv;

  if ((fd_serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    return (84);
  addr_serv.sin_family = AF_INET;
  addr_serv.sin_addr.s_addr = inet_addr(client->ip_serv_string);
  addr_serv.sin_port = htons(atoi(client->port_serv));
  if (connect(fd_serv, (struct sockaddr *)&addr_serv,
	      sizeof(addr_serv)) == -1)
    close(fd_serv);
  else
    {
      client->connect = 1;
      client->fd_serv = fd_serv;
      printf("Connected to %s:%s\n",
	     client->ip_serv_string, client->port_serv);
    }
  return (0);
}

int			main(void)
{
  char			buff[1024];
  t_client		client;
  int			end;

  client.connect = 0;
  while (!client.connect)
    {
      if ((end = read(0, buff, 1023)) == -1)
	return (84);
      buff[((end > 0) ? end - 1 : end)] = 0;
      if (!get_connect(&client, buff))
	{
	  if (connecting(&client))
	    return (84);
	}
      else
	printf("%s\n", ERROR_CONNECT);
    }
  while (1)
    {
      end = read(client.fd_serv, buff, 1023);
      buff[end] = 0;
      printf("%s\n", buff);
    }
  return (0);
}
