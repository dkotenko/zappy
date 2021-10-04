# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clala <clala@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/16 16:31:55 by gmelisan          #+#    #+#              #
#    Updated: 2021/10/04 18:27:40 by clala            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

CC = clang
CFLAGS = -g -Wall -Wextra #-O3

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
MAKE_LIB = make -C $(LIBFT_DIR)

SRCDIR = src
SRC = $(addprefix $(SRCDIR)/, \
main.c \
server.c logger.c utils.c circbuf.c \
ft_btree.c commands.c logic.c reception.c \
graphic.c admin.c \
logic/actions_client.c logic/game_cycle.c \
logic/json.c logic/map.c logic/random.c logic/t_aux.c \
logic/main.c \
)

HEADER = $(addprefix $(SRCDIR)/, \
server.h logger.h utils.h circbuf.h \
ft_btree.h commands.h reception.h \
graphic.h admin.h \
logic/color.h logic/const.h logic/zappy.h)


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -o $(NAME) $^

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

