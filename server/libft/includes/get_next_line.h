/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 15:15:19 by clala             #+#    #+#             */
/*   Updated: 2021/04/16 23:35:39 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft_basic.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/select.h>
# include <stdio.h>

# define BUFF_SIZE 32

typedef struct s_gnl
{
	int		last_fd;
	char	stack[BUFF_SIZE + 1];
	char	*fd_arr[FD_SETSIZE + 1];
}	t_gnl;

int		get_next_line(const int fd, char **line);

#endif
