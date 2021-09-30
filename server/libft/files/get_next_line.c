/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 15:10:51 by clala             #+#    #+#             */
/*   Updated: 2021/04/16 23:18:57 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	copy_to_fd_arr(char **fd_arr, char *stack)
{
	char		*tmp;
	size_t		len;

	if (!*fd_arr)
	{
		*fd_arr = ft_strdup(stack);
		return ;
	}
	len = ft_strlen(*fd_arr);
	tmp = *fd_arr;
	*fd_arr = ft_strnew(len + BUFF_SIZE);
	if (*fd_arr)
	{
		ft_memcpy(*fd_arr, (const void *)tmp, len);
		ft_memcpy(*fd_arr + len, (const void *)stack, BUFF_SIZE + 1);
	}
	free(tmp);
}

static void	get_line_from_fd_arr(char **fd_arr, char **line,
		char *n, int len_before_n)
{
	char		*tmp;
	size_t		len_n;

	*line = ft_strnew(len_before_n + 1);
	ft_memcpy(*line, *fd_arr, len_before_n);
	tmp = *fd_arr;
	len_n = ft_strlen(n);
	*fd_arr = ft_strnew(len_n);
	ft_memcpy(*fd_arr, n + 1, len_n - 1);
	free(tmp);
}

void	init(t_gnl *gnl, int *r, int fd, char **line)
{
	*r = read(fd, gnl->stack, 0);
	if (read(fd, gnl->stack, 0) < 0)
	{
		write(STDERR_FILENO, ERR_FILE_READ, ft_strlen(ERR_FILE_READ));
		exit(0);
	}
	if (fd < 0 || fd > FD_SETSIZE)
	{
		write(STDERR_FILENO, ERR_INV_FD, ft_strlen(ERR_INV_FD));
		exit(0);
	}
	*line = NULL;
	if (!gnl->fd_arr[fd])
		gnl->fd_arr[fd] = ft_strnew(0);
}

int	get_next_line(int const fd, char **line)
{
	static t_gnl	gnl;
	int				r;
	char			*n;

	init(&gnl, &r, fd, line);
	while (!(ft_strchr(gnl.fd_arr[fd], '\n')))
	{
		r = read(fd, gnl.stack, BUFF_SIZE);
		if (!r)
			break ;
		copy_to_fd_arr(&gnl.fd_arr[fd], gnl.stack);
		ft_strclr(gnl.stack);
	}
	n = ft_strchr(gnl.fd_arr[fd], '\n');
	if (n)
		get_line_from_fd_arr(&gnl.fd_arr[fd], line, n, n - gnl.fd_arr[fd]);
	else
	{
		*line = gnl.fd_arr[fd];
		gnl.fd_arr[fd] = NULL;
	}
	if (!r && !gnl.fd_arr[fd] && *line[0] == '\0')
		ft_strdel(line);
	return (*line != NULL);
}
