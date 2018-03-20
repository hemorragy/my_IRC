##
## Makefile for malloc in /home/chouik_e/Tek/Tek2/PSU_2016_malloc
##
## Made by Eddy Chouikha
## Login   <chouik_e@epitech.net>
##
## Started on  Wed Jan 25 17:06:48 2017 Eddy Chouikha
## Last update Sun Jun 11 18:51:11 2017 Loïs Antoine

CC	=	gcc -g3

RM	=	rm -rf

CFLAGS	+=	-Wextra -Wall -Werror -Iinclude

NAME1	=	server

NAME2	=	client

SRC	=	srcs/

SERV	=	$(SRC)server/

CLI	=	$(SRC)client/

SRCS1	=	$(SERV)main_server.c \
		$(SERV)read_server.c \
		$(SERV)chan_server.c \
		$(SERV)chan2_server.c \
		$(SERV)basic_server.c \
		$(SERV)get_next_line.c \
		$(SERV)message_server.c \
		$(SERV)identify_server.c \
		$(SERV)identify2_server.c \
		$(SERV)chan_tools_server.c

OBJS1	=	$(SRCS1:.c=.o)

SRCS2	=	$(CLI)main_client.c \
		$(CLI)connect_client.c

OBJS2	=	$(SRCS2:.c=.o)

COM	=	$(SRC)utils.c

COBJS	=	$(COM:.c=.o)

all:	$(NAME1) $(NAME2)

$(NAME1):	$(OBJS1) $(COBJS)
		$(CC) -o $(NAME1) $(OBJS1) $(COBJS)

$(NAME2):	$(OBJS2) $(COBJS)
		$(CC) -o $(NAME2) $(OBJS2) $(COBJS)

clean:
	$(RM) $(OBJS1)
	$(RM) $(OBJS2)
	$(RM) $(COBJS)

fclean: 	clean
		$(RM) $(NAME1)
		$(RM) $(NAME2)

re:	fclean all

.PHONY:	all clean fclean re
