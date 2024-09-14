# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jurodrig <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/29 12:19:21 by jurodrig          #+#    #+#              #
#    Updated: 2024/08/30 12:53:01 by jurodrig         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### Colors ###
GREEN   =   \033[0;32m
RED     =   \033[0;31m
CYAN    =   \033[0;36m
WHITE   =   \033[0m

NAME_C  =   client
NAME_S  =   server

CLIENT_SRC  =   client.c main_client.c miniutils.c
SERVER_SRC  =   server.c main_server.c

SERVER_OBJ  =	$(SERVER_SRC:.c=.o)	
CLIENT_OBJ  =   $(CLIENT_SRC:.c=.o)

CC      =   gcc

CFLAGS  =   -Wall -Wextra -Werror
CFLAGS  +=  -I.
CFLAGS  +=  -I./library

LIBFT   =   ./library/libft.a

all:    $(NAME_C) $(NAME_S)

$(NAME_C): $(CLIENT_OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(CLIENT_OBJ) $(LIBFT) -o $(NAME_C)
	@echo "$(GREEN)CLIENT READY$(WHITE)"

$(NAME_S): $(SERVER_OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(SERVER_OBJ) $(LIBFT) -o $(NAME_S)
	@echo "$(CYAN)SERVER READY$(WHITE)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C library

clean:
	@make -C library clean
	@rm -f $(CLIENT_OBJ) $(SERVER_OBJ)
	@echo "$(CYAN)Object files cleaned$(WHITE)"

fclean: clean
	@make -C library fclean
	@rm -f $(NAME_C) $(NAME_S)
	@echo "$(CYAN)Executables cleaned$(WHITE)"

re: fclean all

.PHONY: all clean fclean re

