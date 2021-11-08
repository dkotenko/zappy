/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:46:01 by clala             #+#    #+#             */
/*   Updated: 2021/04/16 23:36:19 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

t_buf					*g_buf;
t_var					g_v;

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		written;

	va_start(ap, format);
	written = ft_vdprintf(1, format, &ap);
	va_end(ap);
	return (written);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		written;

	va_start(ap, format);
	written = ft_vdprintf(fd, format, &ap);
	va_end(ap);
	return (written);
}

int	ft_asprintf(char **s, const char *format, ...)
{
	int		written;
	va_list	ap;

	va_start(ap, format);
	written = ft_vasprintf(s, format, &ap);
	va_end(ap);
	return (written);
}

void	printf_print_format_handler(const char *format, int *i)
{
	if (g_v.zero_sign && !g_v.minus_flag)
		g_v.width_sign = '0';
	else
		g_v.width_sign = ' ';
	while (g_v.width - 1 > 0 && !g_v.minus_flag)
	{
		t_buf_write(g_buf, &g_v.width_sign, 1);
		g_v.width--;
	}
	t_buf_write(g_buf, &format[*i + g_v.len_format + 1], 1);
	(*i)++;
	while (g_v.width-- - 1 > 0)
		t_buf_write(g_buf, &g_v.width_sign, 1);
}
