/*
** irc.h for irc in /home/antoin_g/rendu/Unix/PSU_2016_myirc
**
** Made by Loïs Antoine
** Login   <antoin_g@pc-antoin_g>
**
** Started on  Wed May 24 17:08:03 2017 Loïs Antoine
** Last update Sun Jun 11 18:58:58 2017 Loïs Antoine
*/

#ifndef IRC_H_
# define IRC_H_

# include <stdbool.h>
# include <time.h>

/*
** All
*/
char		*epure_str(char *);
char		**my_str_to_wordtab(const char *);

/*
** Server
*/
# define FD_FREE 0
# define FD_CLIENT 1
# define FD_SERVER 2
# define MAX_FD 255

# define M001_1 "001"
# define M001_2 ":Welcome to the My_IRC"
# define M002_1 "002"
# define M002_2 ":Your host is myirc, running version 1.0"
# define M003_1 "003"
# define M003_2 ":This server was created"
# define M321_1 "321"
# define M321_2 "Channel :Users Name"
# define M322 "322"
# define M323_1 "323"
# define M323_2 ":End of channel list."
# define M353 "353"
# define M366_1 "366"
# define M366_2 ":End of /NAMES list."

# define E401_1 "401"
# define E401_2 ":No such nick/channel"
# define E403_1 "403"
# define E403_2 ":No such channel"
# define E404_1 "404"
# define E404_2 ":Cannot send to channel"
# define E421_1 "421"
# define E421_2 ":Unknown command"
# define E432_1 "432"
# define E432_2 ":Erroneus nickname"
# define E433_1 "433"
# define E433_2 ":Nickname is already in use"
# define E451_1 "451"
# define E451_2 ":You have not registered"
# define E461_1 "461"
# define E461_2 ":Not enough parameters"
# define E462_1 "462"
# define E462_2 ":You may not reregister"

#ifndef READ_SIZE
# define READ_SIZE (1024)
#endif /* !READ_SIZE */

typedef struct	s_gnl
{
  int		test;
  char		*line;
}		t_gnl;

typedef struct		s_chan
{
  int			nb;
  char			*name;
  struct s_chan		*next;
}			t_chan;

typedef int	(*t_fct)();

typedef struct	s_identity
{
  bool		regis;
  bool		ping_s;
  bool		ping_r;
  char		ping[11];
  char		*nick;
  char		*user;
  char		*host;
  char		*serv;
  char		*real;
}		t_identity;

typedef struct	s_serv_info
{
  int		port;
  time_t	creating_date;
}		t_serv_info;

typedef struct	s_env
{
  char		fd_type[MAX_FD];
  t_fct		fct[MAX_FD];
  t_identity	iden[MAX_FD];
  t_chan	*chan_location[MAX_FD];
  t_chan	*chan_list;
  char		buff_list[MAX_FD][READ_SIZE + 1];
  t_serv_info	info;
}		t_env;

typedef bool	(*t_command)(t_env *, char *, int);

typedef struct	s_comm
{
  char		*command;
  t_command	func;
}		t_comm;

int	add_client(t_env *, int);
int	client_read(t_env *, int);
int	server_read(t_env *, int);
int	chan_users(const t_env *, const char *);
bool	chan_exist(t_chan *, const char *);
bool	command_nick(t_env *, char *, int);
bool	command_user(t_env *, char *, int);
bool	command_quit(t_env *, char *, int);
bool	command_pong(t_env *, char *, int);
bool	command_join(t_env *, char *, int);
bool	command_part(t_env *, char *, int);
bool	command_list(t_env *, char *, int);
bool	command_pmsg(t_env *, char *, int);
bool	command_name(t_env *, char *, int);
char	*get_next_line(int, t_env *);
void	free_all_chan(t_chan *);
t_chan	*free_chan(t_chan *, const char *);
t_chan	*add_channel(t_chan *, const char *);

/*
** Client
*/
# define ERROR_CONNECT "USAGE: /server $host[:$port]"

typedef struct	s_client
{
  int		fd_serv;
  char		ip_serv_string[16];
  char		ip_serv[4][4];
  char		port_serv[6];
  bool		connect;
}		t_client;

int	get_connect(t_client *, const char *);

#endif /* !IRC_H_ */
