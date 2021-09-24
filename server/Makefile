#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/16 16:31:55 by gmelisan          #+#    #+#              #
#    Updated: 2021/09/24 17:06:57 by gmelisan         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = server

CC = clang
CFLAGS = -g -Wall -Wextra

SRCDIR = src
SRC = $(addprefix $(SRCDIR)/, \
main.c server.c logger.c utils.c circbuf.c queue.c)

HEADER = $(addprefix $(SRCDIR)/, \
server.h logger.h utils.h circbuf.h queue.h)

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $^

$(OBJ): $(HEADER)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -f $(OBJ)
	@rm -rf $(NAME).dSYM

fclean: clean
	@rm -f $(NAME)

re: fclean all

