# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clala <clala@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/16 16:31:55 by gmelisan          #+#    #+#              #
#    Updated: 2021/10/10 17:56:56 by clala            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test

CC = clang
CFLAGS = -g -Wall -Wextra #-O3


SERVER_PATH = $(subst /src/logic/tests,,$(CURDIR))
BASE_PATH = $(subst /logic/tests,,$(CURDIR))
LOGIC_PATH = $(addprefix $(BASE_PATH), /logic)
TESTS_PATH = $(addprefix $(LOGIC_PATH), /tests)

LIBFT_DIR = $(SERVER_PATH)/libft
LIBFT = $(LIBFT_DIR)/libft.a
MAKE_LIB = make -C $(LIBFT_DIR)

SRCDIR = src
SRC_NAMES = $(BASE_PATH)/server.c \
			$(BASE_PATH)/logger.c \
			$(BASE_PATH)/utils.c \
			$(BASE_PATH)/circbuf.c \
			$(BASE_PATH)/ft_btree.c \
			$(BASE_PATH)/commands.c \
			$(BASE_PATH)/reception.c \
			$(BASE_PATH)/graphic.c \
			$(BASE_PATH)/admin.c \
			$(LOGIC_PATH)/actions_client.c \
			$(LOGIC_PATH)/game_cycle.c \
			$(LOGIC_PATH)/json.c \
			$(LOGIC_PATH)/map.c \
			$(LOGIC_PATH)/random.c \
			$(LOGIC_PATH)/t_aux.c \
			$(LOGIC_PATH)/game.c \
			$(LOGIC_PATH)/print.c \
			\
			$(TESTS_PATH)/test.c

HEADER_NAMES = $(BASE_PATH)/server.h \
			$(BASE_PATH)/logger.h \
			$(BASE_PATH)/utils.h \
			$(BASE_PATH)/circbuf.h \
			$(BASE_PATH)/ft_btree.h \
			$(BASE_PATH)/commands.h \
			$(BASE_PATH)/reception.h \
			$(BASE_PATH)/graphic.h \
			$(BASE_PATH)/admin.h \
			$(LOGIC_PATH)/color.h \
			$(LOGIC_PATH)/const.h \
			$(LOGIC_PATH)/zappy.h \
			$(LOGIC_PATH)/actions_client.c


HEADER = $(HEADER_NAMES)
SRC = $(SRC_NAMES)
OBJ = $(SRC:.c=.o)



all: $(LIBFT) $(NAME)

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
	@rm -rf $(BASE_PATH)/main.o
	$(MAKE_LIB) clean

fclean: clean
	@rm -f $(NAME)
	$(MAKE_LIB) fclean

re: fclean all

