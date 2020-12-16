# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ojoubout <ojoubout@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/10 10:53:07 by ojoubout          #+#    #+#              #
#    Updated: 2019/11/23 22:10:50 by ojoubout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell

SRC = srcs/minishell.c srcs/ft_sh_split.c
SRC += gnl/get_next_line.c

OBJ = $(SRC:.c=.o)

FLAGS=-Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	@gcc $(FLAGS) $(SRC) libft/libft.a -Iincludes -o $(NAME)

clean:


fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: fclean clean all re
