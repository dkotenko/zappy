# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clala <clala@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/16 16:31:55 by gmelisan          #+#    #+#              #
#    Updated: 2021/10/09 13:28:41 by clala            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test

CC = clang
CFLAGS = -g -Wall -Wextra #-O3

LIBFT_DIR = /Users/clala/Desktop/REPOs/zappy/server/libft
LIBFT = $(LIBFT_DIR)/libft.a
MAKE_LIB = make -C $(LIBFT_DIR)

SRCDIR = src
SRC_NAMES = /Users/clala/Desktop/REPOs/zappy/server/src/server.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logger.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/utils.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/circbuf.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/ft_btree.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/commands.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/reception.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/graphic.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/admin.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/actions_client.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/game_cycle.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/json.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/map.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/random.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/t_aux.c \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/game.c \
			\
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/tests/main.c

HEADER_NAMES = /Users/clala/Desktop/REPOs/zappy/server/src/server.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/logger.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/utils.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/circbuf.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/ft_btree.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/commands.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/reception.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/graphic.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/admin.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/color.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/const.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/zappy.h \
			/Users/clala/Desktop/REPOs/zappy/server/src/logic/actions_client.c


HEADER = $(HEADER_NAMES)
SRC = $(SRC_NAMES)
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L$(LIBFT_DIR) -lft

$(OBJ): $(HEADER)

$(LIBFT):
	$(MAKE_LIB)

%.o : %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes -o $@ -c $<

clean:
	@rm -f $(OBJ)
	@rm -rf $(NAME).dSYM
	$(MAKE_LIB) clean

fclean: clean
	@rm -f $(NAME)
	$(MAKE_LIB) fclean

re: fclean all

