/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:46:14 by clala             #+#    #+#             */
/*   Updated: 2021/04/16 23:36:58 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

extern t_buf	*g_buf;
extern t_var	g_v;

static int	printf_print_format(const char *format,
va_list *ap, int *i)
{
	if (!g_v.type_spec && g_v.len_format && format[*i + g_v.len_format + 1])
		printf_print_format_handler(format, i);
	else if (!g_v.type_spec && !g_v.len_format)
	{
		if (ft_strlen(format) != 1)
			t_buf_write(g_buf, &format[*i], 1);
	}
	else if (ft_strchr("diouxXb", g_v.type_spec))
		printf_diouxxb(ap);
	else if (ft_strchr("cspr", g_v.type_spec))
		printf_cspr(ap);
	else if (g_v.type_spec == 'f')
		printf_f(ap);
	return (1);
}

static void	initializer(int *i, int *flag_to_print,
		int *format_len, const char *format)
{
	*flag_to_print = 0;
	*i = 0;
	*format_len = ft_strlen(format);
	g_v.c_zero = 0;
	g_buf = t_buf_create(T_BUF_BUFF_SIZE);
}

static int	set_flag_to_print(int *flag_to_print, char to_check)
{
	if (!to_check)
	{
		*flag_to_print = 1;
		return (1);
	}
	return (0);
}

int	ft_vdprintf(int fd, const char *format, va_list *ap)
{
	int		i;
	int		format_len;
	int		flag_to_print;
	int		written;

	initializer(&i, &flag_to_print, &format_len, format);
	while (format[i] && i < format_len)
	{
		while (format[i] != '%')
		{
			if (set_flag_to_print(&flag_to_print, format[i]))
				break ;
			t_buf_write(g_buf, &format[i++], 1);
		}
		if (!flag_to_print && f_set_globals_default())
		{
			if (handle_format(format, ap, &i))
				printf_print_format(format, ap, &i);
			i += g_v.len_format + 1;
		}
	}
	written = write(fd, g_buf->s, t_buf_len(g_buf->s));
	t_buf_free(&g_buf);
	return (written);
}

int	ft_vasprintf(char **s, const char *format, va_list *ap)
{
	int		i;
	int		format_len;
	int		flag_to_print;

	initializer(&i, &flag_to_print, &format_len, format);
	while (format[i] && i < format_len)
	{
		while (format[i] != '%')
		{
			if (set_flag_to_print(&flag_to_print, format[i]))
				break ;
			t_buf_write(g_buf, &format[i++], 1);
		}
		if (!flag_to_print && f_set_globals_default())
		{
			if (handle_format(format, ap, &i))
				printf_print_format(format, ap, &i);
			i += g_v.len_format + 1;
		}
	}
	*s = ft_strnew(t_buf_len(g_buf->s));
	ft_memcpy(*s, g_buf->s, t_buf_len(g_buf->s));
	t_buf_free(&g_buf);
	return (ft_strlen(*s));
}
