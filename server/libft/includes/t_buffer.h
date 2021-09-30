/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_buffer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:48:19 by clala             #+#    #+#             */
/*   Updated: 2021/09/26 14:10:40 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_BUFFER_H
# define T_BUFFER_H

# include "libft_basic.h"
# define T_BUFFER_BUFF_SIZE 0xFF
# define T_BUFFER_GETCHAR_BUFFSIZE 1

typedef struct s_buffer
{
	int			size;
	int			i;
	char		*s;
}				t_buffer;

void			t_buffer_add_size(t_buffer *buf);
t_buffer		*t_buffer_create(int size);
void			t_buffer_free(t_buffer **buf);
int				t_buffer_write(t_buffer *buf, const char *str);
void			t_buffer_clean(t_buffer *buf);
char			t_buffer_pop(t_buffer *buf);
char			t_buffer_getchar(t_buffer *buf);
int				t_buffer_add_char(t_buffer *buf, char c);
int				t_buffer_writen(t_buffer *buf, const char *str, int n);
int				t_buffer_write_int(t_buffer *buf, int n);

#endif
