# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ojoubout <ojoubout@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/11 13:17:23 by ojoubout          #+#    #+#              #
#    Updated: 2019/11/05 20:36:46 by ojoubout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libftprintf.a

LBFOBG = ft_memset.o ft_bzero.o ft_memcpy.o ft_memccpy.o ft_memmove.o ft_memchr.o \
	ft_memcmp.o ft_strlen.o ft_strlcpy.o ft_strlcat.o ft_strchr.o ft_strrchr.o \
	ft_strnstr.o ft_strncmp.o ft_atoi.o ft_isalpha.o ft_isdigit.o ft_isalnum.o \
	ft_isascii.o ft_isprint.o ft_toupper.o ft_tolower.o ft_calloc.o ft_strdup.o \
	ft_substr.o ft_strjoin.o ft_strtrim.o ft_split.o ft_itoa.o ft_strmapi.o \
	ft_putchar_fd.o ft_putstr_fd.o ft_putendl_fd.o ft_putnbr_fd.o \
	ft_conversion_c.o ft_conversion_s.o ft_convert.o ft_flags.o ft_putnbr.o \
	ft_conversion_d_i.o ft_conversion_u.o ft_field_width.o ft_precision.o ft_putstr.o \
	ft_conversion_p.o ft_conversion_x.o ft_flag_zero.o ft_printf.o ft_uitoa.o


all: $(NAME)

$(NAME):
	@make -C libft all
	@gcc -c ft*.c -I.
	@ar -x libft/libft.a
	@ar rc $(NAME) $(LBFOBG)
	@ranlib $(NAME)

clean:
	@make -C libft clean
	@rm -f $(LBFOBG)

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all
