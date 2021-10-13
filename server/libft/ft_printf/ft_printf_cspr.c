/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_cspr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:46:22 by clala             #+#    #+#             */
/*   Updated: 2020/02/20 18:59:05 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

extern t_buf	*g_buf;
extern t_var	g_v;

static void	printf_s(char *s)
{
	int		len;

	if (!s)
		s = (char *)"(null)";
	len = ft_strlen(s);
	s = ft_strdup(s);
	if (g_v.precis > -1 && g_v.precis < len)
	{
		s[g_v.precis] = '\0';
		len = g_v.precis;
	}
	if (g_v.minus_flag)
	{
		t_buf_write(g_buf, s, len);
		while (g_v.width-- > len)
			t_buf_write(g_buf, " ", 1);
	}
	else
	{
		while (g_v.width-- > len)
			t_buf_write(g_buf, " ", 1);
		t_buf_write(g_buf, s, len);
	}
	free(s);
}

static void	printf_c(va_list *ap)
{
	char	c;

	c = va_arg(*ap, unsigned int);
	if (g_v.minus_flag)
	{
		t_buf_write(g_buf, &c, 1);
		while (g_v.width-- > 1)
			t_buf_write(g_buf, " ", 1);
	}
	else
	{
		while (g_v.width-- > 1)
			t_buf_write(g_buf, " ", 1);
		t_buf_write(g_buf, &c, 1);
	}
}

static void	printf_p_right(int len, void *x, char *s)
{
	while (g_v.width > ft_imaxval("2", len, g_v.precis))
	{
		g_v.width--;
		t_buf_write(g_buf, " ", 1);
	}
	t_buf_write(g_buf, "0x", 2);
	while (g_v.precis > len)
	{
		g_v.precis--;
		t_buf_write(g_buf, "0", 1);
	}
	if (x || g_v.precis)
		t_buf_write(g_buf, s, len);
	while (g_v.width-- > len)
	{
		g_v.width--;
		t_buf_write(g_buf, " ", 1);
	}
}

static void	printf_p(va_list *ap)
{
	void	*x;
	char	*s;
	int		len;

	x = va_arg(*ap, void *);
	s = ft_strlwr(ft_uiptrtoa_base((uintptr_t)x, 16));
	len = ft_strlen(s);
	g_v.width -= 2;
	if (g_v.width < 0)
		g_v.width = 0;
	if (g_v.minus_flag)
	{
		t_buf_write(g_buf, "0x", 2);
		while (g_v.precis > len && g_v.precis--)
			t_buf_write(g_buf, "0", 1);
		t_buf_write(g_buf, s, len);
		while (g_v.width > len)
		{
			g_v.width--;
			t_buf_write(g_buf, " ", 1);
		}
	}
	else
		printf_p_right(len, x, s);
	free(s);
}

void	printf_cspr(va_list *ap)
{
	char	*s;

	if (g_v.type_spec == 'c')
		printf_c(ap);
	else if (g_v.type_spec == 's')
	{
		s = va_arg(*ap, char *);
		printf_s(s);
	}
	else if (g_v.type_spec == 'p')
		printf_p(ap);
	else if (g_v.type_spec == 'r')
		printf_r(ap);
}
