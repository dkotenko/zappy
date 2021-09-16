/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:19:52 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/16 21:03:27 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>

#include "utils.h"
#include "logger.h"

void xassert(int value, const char *str)
{
	if (!value) {
		log_fatal("%s: %s", str, strerror(errno));
	}
}
