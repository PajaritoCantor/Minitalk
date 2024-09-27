
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: psegura- <psegura-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/06 16:12:27 by psegura-          #+#    #+#              #
#    Updated: 2024/08/17 00:15:24 by psegura-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory

### Colors ###

GREEN	=	\033[1;32m
RED		=	\033[0;31m
CYAN	=	\033[0;36m
WHITE	=	\033[0m

NAME_C = client
NAME_S = server

SRCS_SHARED = $(addprefix shared/, $(SHARED))

CLIENT =							\
			client/main.c			\
			client/client.c			\
			client/process_input.c			\
			client/check_server_status.c			\
			$(SRCS_SHARED)			\
		
SRCS_C = $(addprefix srcs/, $(CLIENT))

SERVER =							\
			server/main.c			\
			server/server.c			\
			server/check_client_status.c			\
			$(SRCS_SHARED)			\

SRCS_S = $(addprefix srcs/, $(SERVER))

OBJS_C = $(SRCS_C:%.c=objs/%.o)
OBJS_S = $(SRCS_S:%.c=objs/%.o)

LIB = libft/libft.a

CC = cc

CFLAGS	 = -Wall -Wextra -Werror
CFLAGS	+= -I inc
CFLAGS	+= -I libft

all: $(NAME_C) $(NAME_S)

$(NAME_C): objs $(OBJS_C)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJS_C) $(LIB) -o $(NAME_C)
	@echo "$(RED)CLIENT READY$(WHITE)"

$(NAME_S): objs $(OBJS_S)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJS_S) $(LIB) -o $(NAME_S)
	@echo "$(GREEN)SERVER READY$(WHITE)"

objs:
	@mkdir -p objs/srcs/server objs/srcs/client

objs/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C libft
	@rm -rf objs

fclean: clean
	@make fclean -C libft
	@rm -f $(NAME_C)
	@rm -f $(NAME_S)

re:: fclean
re:: all

.PHONY: all clean fclean re norma debug
