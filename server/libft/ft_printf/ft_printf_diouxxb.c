/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_diouxxb.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 20:46:29 by clala             #+#    #+#             */
/*   Updated: 2020/02/20 18:59:05 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

extern t_buf	*g_buf;
extern t_var	g_v;

static int	get_base(void)
{
	if (g_v.type_spec == 'o')
		return (8);
	else if (g_v.type_spec == 'x' || g_v.type_spec == 'X')
		return (16);
	else if (g_v.type_spec == 'b')
		return (2);
	else
		return (10);
}

static char	*handle_diouxxb_string(t_data value)
{
	char	*s;

	s = NULL;
	if (!g_v.size_spec)
		s = ft_itoa_base(value.int_, get_base());
	else if (g_v.size_spec == 'l' || g_v.size_spec == 'l' * 2)
		s = ft_ltoa_base(value.long_, get_base());
	else if (g_v.size_spec == 'h')
		s = ft_itoa_base(value.short_, get_base());
	else if (g_v.size_spec == 'h' * 2)
		s = ft_itoa_base(value.char_, get_base());
	return (s);
}

static char	*get_diouxxb_string(t_data value)
{
	char	*s;

	s = NULL;
	if (g_v.type_spec == 'd' || g_v.type_spec == 'i')
	{
		s = handle_diouxxb_string(value);
	}
	else
	{
		if (!g_v.size_spec)
			s = ft_uitoa_base(value.uint_, get_base());
		else if (g_v.size_spec == 'l' || g_v.size_spec == 'l' * 2)
			s = ft_ultoa_base(value.ulong_, get_base());
		else if (g_v.size_spec == 'h')
			s = ft_uitoa_base(value.ushort_, get_base());
		else if (g_v.size_spec == 'h' * 2)
			s = ft_uitoa_base(value.uchar_, get_base());
	}
	return (s);
}

void	printf_diouxxb(va_list *ap)
{
	char		*s;
	t_data		value;

	value.intmax_ = va_arg(*ap, intmax_t);
	if (g_v.size_spec == 'L')
		g_v.size_spec = 0;
	s = get_diouxxb_string(value);
	if (g_v.type_spec == 'X')
		s = ft_strupr(s);
	else
		s = ft_strlwr(s);
	diouxxb_printer(s);
	free(s);
}
