# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/10 20:24:40 by egonin            #+#    #+#              #
#    Updated: 2026/03/16 19:25:45 by ltourbe          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I./include -I./libft

SRCS		= src/main.c \
			src/lexer.c \
			src/clear_and_free.c \
			src/execution_free_errors.c \
			src/execution_init.c \
			src/execution_path.c \

OBJ_DIR		= obj
OBJS		= $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

NAME		= minishell

LIBFT		= libft/libft.a

LIBS 		= -lreadline

all: libft $(NAME)	

$(NAME):	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LIBS)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

libft:
		$(MAKE) -C libft

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C libft clean

fclean:		clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean

re:		fclean all

.PHONY: clean fclean all re libft
