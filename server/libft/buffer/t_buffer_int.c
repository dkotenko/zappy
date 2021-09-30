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

int	t_buffer_write_int(t_buffer *buf, int n)
{
	char	*temp;
	int		written;

	temp = ft_itoa(n);
	written = t_buffer_write(buf, temp);
	free(temp);
	return (written);
}
