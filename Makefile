#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/16 16:31:03 by gmelisan          #+#    #+#              #
#    Updated: 2021/09/16 16:48:32 by gmelisan         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

all: server client gfx

server: FORCE
	make -C server

client: FORCE
	@echo placeholder for client 

gfx: FORCE
	@echo placeholder for gfx

clean:
	@make -C server clean
fclean: clean
	@make -C server fclean

re: fclean all

FORCE:
