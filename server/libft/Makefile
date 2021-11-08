# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clala <clala@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/15 16:58:46 by clala             #+#    #+#              #
#    Updated: 2021/10/28 13:04:57 by gmelisan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a
HEADERS_DIR = includes
HEADERS_NAMES=libft.h \
dlist.h \
error.h \
ft_printf.h \
get_next_line.h \
keys.h \
libft.h \
libft_basic.h \
t_buffer.h \
t_htable.h

HEADERS = $(addprefix $(HEADERS_DIR)/, $(HEADERS_NAMES))
include list.mak
OBJ=$(SRCS:.c=.o)
INCLUDES=-I./includes

CC=clang -Wall -Wextra -Werror -g
THREADS = 8


all:
	@$(MAKE) -j$(THREADS) $(NAME)

multi:
	@$(MAKE) -j$(THREADS) all

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ) 
	@echo =================================
	@echo $(NAME) HAS BEEN MADE          
	@echo =================================	

%.o:%.c $(HEADERS)
	@$(CC) $(INCLUDES) -c $< -o $@

clean:
	@/bin/rm -f $(OBJ)
fclean: clean
	@/bin/rm -f $(NAME)
re: fclean all

.PHONY: lib clean fclean all re multi
