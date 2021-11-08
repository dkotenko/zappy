/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_buffer_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:48:19 by clala             #+#    #+#             */
/*   Updated: 2021/02/07 00:08:55 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_buffer.h"

int	t_buffer_add_char(t_buffer *buf, char c)
{
	if (buf->i + 1 >= buf->size)
		t_buffer_add_size(buf);
	buf->s[buf->i++] = c;
	return (buf->i);
}

int	t_buffer_write(t_buffer *buf, const char *str)
{
	int		i;
	int		len;

	len = ft_strlen(str);
	i = 0;
	if (str && str[i])
	{
		while (buf->i + len >= buf->size)
			t_buffer_add_size(buf);
		while (str[i] && i < len)
		{
			buf->s[buf->i++] = str[i++];
		}
	}
	return (i);
}

char	t_buffer_getchar(t_buffer *buf)
{
	char	temp[T_BUFFER_GETCHAR_BUFFSIZE + 1];
	int		has_read;

	ft_memset(temp, 0, T_BUFFER_GETCHAR_BUFFSIZE + 1);
	has_read = read(STDIN_FILENO, temp, T_BUFFER_GETCHAR_BUFFSIZE);
	if (has_read == -1)
		handle_error("Can't read STDIN");
	if (!has_read)
		return (0);
	t_buffer_add_char(buf, temp[0]);
	return (temp[0]);
}

char	t_buffer_pop(t_buffer *buf)
{
	char	c;

	c = 0;
	if (buf->i > -1)
	{
		c = buf->s[buf->i];
		buf->s[buf->i] = '\0';
		if (buf->i > 0)
			buf->i--;
	}
	return (c);
}

void	t_buffer_clean(t_buffer *buf)
{
	ft_strclr(buf->s);
	buf->i = 0;
}
