# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jurodrig <jurodrig@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 20:53:01 by jurodrig          #+#    #+#              #
#    Updated: 2025/01/03 18:15:03 by jurodrig         ###   ########.fr        #
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
			client/validate_arguments.c			\
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
