# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/10 20:24:40 by egonin            #+#    #+#              #
#    Updated: 2026/03/23 19:27:48 by ltourbe          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I./include -I./libft

SRCS		= src/main.c \
			src/lexer/lexer.c \
			src/lexer/lexer_quotes.c \
			src/lexer/lexer_utils.c \
			src/clear_and_free.c \
			src/execution/execution_free_errors.c \
			src/execution/execution.c \
			src/execution/execution_path.c \
			src/execution/execution_builtins.c \
			src/execution/execution_redirection_builtin.c \
			src/execution/execution_struct.c \
			src/env/env_init.c \
			src/env/env_utils.c \
			src/env/env_utils2.c \
			src/parser/parser.c \
			src/parser/parser_utils.c \
			src/parser/parser_word_pipe.c \
			src/builtins/ft_echo.c \
			src/builtins/ft_exit.c \
			src/builtins/ft_unset.c \
			src/builtins/ft_cd.c \
			src/builtins/ft_env.c \
			src/builtins/ft_pwd.c

OBJ_DIR		= obj
OBJS		= $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

NAME		= minishell

LIBFT		= libft/libft.a

LIBS 		= -lreadline

all: libft $(NAME)	

$(NAME):	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LIBS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
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
