/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_buf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:48:19 by clala             #+#    #+#             */
/*   Updated: 2020/02/20 18:58:17 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	t_buf_add_size(t_buf *buf)
{
	char	*new;
	int		i;

	i = 0;
	new = ft_strnew(T_BUF_BUFF_SIZE + buf->len);
	while (buf->s[i])
	{
		new[i] = buf->s[i];
		i++;
	}
	buf->i = i;
	buf->len += T_BUF_BUFF_SIZE;
	free(buf->s);
	buf->s = new;
}

t_buf	*t_buf_create(int size)
{
	t_buf	*buf;

	buf = (t_buf *)ft_memalloc(sizeof(t_buf));
	if (!size)
		size = T_BUFFER_BUFF_SIZE;
	buf->s = ft_strnew(size);
	buf->i = 0;
	buf->len = size;
	return (buf);
}

void	t_buf_free(t_buf **buf)
{
	free((*buf)->s);
	free(*buf);
}

int	t_buf_len(char *s)
{
	size_t	len;

	f_set_globals_default();
	if (g_v.c_zero)
	{
		len = 0;
		while (g_v.c_zero > 0)
		{
			if (s[len] == 0)
				g_v.c_zero--;
			if (s[len])
				len = len + ft_strlen(s + len);
			else
				len++;
		}
		if (s[len])
			len += ft_strlen(s + len);
		return (len);
	}
	return (ft_strlen(s));
}

int	t_buf_write(t_buf *buf, const char *str, int len)
{
	int	i;

	i = 0;
	if (str[i])
	{
		while (buf->i + len >= buf->len)
			t_buf_add_size(buf);
		while (str[i] && i < len)
		{
			buf->s[buf->i++] = str[i++];
		}
	}
	else if (g_v.type_spec == 'c')
	{
		if (buf->i + 1 >= buf->len)
			t_buf_add_size(buf);
		buf->i++;
		i++;
		(g_v.c_zero)++;
	}
	return (i);
}
