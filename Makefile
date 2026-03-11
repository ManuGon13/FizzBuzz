# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/10 20:24:40 by egonin            #+#    #+#              #
#    Updated: 2026/03/11 18:50:52 by ltourbe          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

SRCS		= src/first.c

	  
OBJS		= $(SRCS:.c=.o)

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

INCS		= -I. -I$(LIBFT_DIR)

LIBS		= -lreadline

NAME		= minishell

all:		$(NAME)	

$(NAME):	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LIBS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o:		%.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean:		clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re:		fclean all

.PHONY: clean fclean all re
