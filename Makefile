# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ojoubout <ojoubout@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/10 10:53:07 by ojoubout          #+#    #+#              #
#    Updated: 2020/12/31 14:50:17 by ojoubout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell

SRC = srcs/minishell.c srcs/ft_sh_split.c srcs/handle_cmd.c srcs/handle_sig.c \
	srcs/redirect.c srcs/handle_exec.c srcs/env_var.c
SRC += gnl/get_next_line.c

OBJ = $(SRC:.c=.o)

FLAGS=-Wall -Wextra -Werror -g3

all: $(NAME)

CC=clang

$(NAME):
	@make CC=$(CC) -C libft bonus
	@make CC=$(CC) -C ft_printf all
	@$(CC) $(FLAGS) $(SRC) ft_printf/libftprintf.a libft/libft.a -Iincludes -o $(NAME)

clean:
	@make -C libft clean
	@make -C ft_printf clean


fclean: clean
	@make -C libft fclean
	@make -C ft_printf fclean

	@rm -rf $(NAME)

re: fclean all

.PHONY: fclean clean all re
