/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_format.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:46:18 by clala             #+#    #+#             */
/*   Updated: 2020/02/20 18:53:04 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

extern t_buf	*g_buf;
extern t_var	g_v;

static int	select_width(const char *format, va_list *ap, int *i)
{
	int		len_width;
	int		j;

	j = *i + g_v.len_format + 1;
	len_width = 0;
	if (ft_isdigit(format[j]))
		g_v.width = ft_atoi(format + j);
	else if (format[j] == '*' && ++len_width)
		g_v.width = va_arg(*ap, int);
	if (!g_v.width)
		return (len_width);
	if (format[j] == '*')
		len_width = 1;
	else
		len_width = ft_ilen(g_v.width);
	g_v.params_changed = 1;
	return (len_width);
}

static int	get_len_prec(char format, int k)
{
	int	len_prec;

	if (g_v.precis > 0)
	{
		if (format == '*')
			len_prec = 2;
		else
			len_prec = ft_ilen(g_v.precis) + 1 + k;
	}	
	else if (g_v.precis > -1)
	{
		if (format == '*')
			len_prec = 2;
		else
			len_prec = ft_ilen(g_v.precis) + k;
	}
	else
		len_prec = 1;
	if (len_prec < 2)
		g_v.precis = 0;
	return (len_prec);
}

static int	select_precision(const char *format, va_list *ap, int *i)
{
	int		j;
	int		len_prec;
	int		k;

	j = *i + g_v.len_format + 1;
	k = 0;
	len_prec = 0;
	if (format[j] == '.')
	{
		g_v.params_changed = 1;
		if (ft_isdigit(format[j + 1]))
		{
			g_v.precis = ft_atoi(format + j + 1);
			while (g_v.precis > -1 && format[j + 1 + k] && \
				format[j + 1 + k] == '0')
				k++;
		}
		else if (format[j + 1] == '*')
			g_v.precis = va_arg(*ap, int);
		len_prec = get_len_prec(format[j + 1], k);
	}
	return (len_prec);
}

static int	select_flag(const char *format, int *i)
{
	int		len_flag;
	char	c;

	len_flag = 0;
	c = format[*i + g_v.len_format + len_flag + 1];
	while (c == '#' || c == '-' || c == '+' || c == ' ' || c == '0')
	{
		if (c == '#')
			g_v.sharp_sign = 1;
		else if (c == '-')
			g_v.minus_flag = 1;
		else if (c == '+')
			g_v.plus_sign = 1;
		else if (c == ' ')
			g_v.space_sign = 1;
		else if (c == '0')
			g_v.zero_sign = 1;
		len_flag++;
		c = format[*i + g_v.len_format + len_flag + 1];
	}
	if (len_flag)
		g_v.params_changed = 1;
	return (len_flag);
}

int	handle_format(const char *format, va_list *ap, int *i)
{
	if (format[*i] == '%')
	{
		if (format[*i + 1] == '%' && t_buf_write(g_buf, "%%", 1))
		{
			(*i)++;
			return (0);
		}
		g_v.len_format += select_flag(format, i);
		g_v.len_format += select_width(format, ap, i);
		g_v.len_format += select_precision(format, ap, i);
		g_v.len_format += select_size_spec(format, i);
		g_v.len_format += select_type_spec(format, i);
	}
	return (1);
}
