/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:22:49 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 18:46:53 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <sys/time.h>

int xassert(int value, const char *str);
void timerprint(void (*f)(const char *format, ...), struct timeval *t, char *name);
int str_starts_with(char *str, char *with);

#endif

