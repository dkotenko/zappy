/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:34:22 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/24 17:37:39 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H


/* Calls when one time unit elapsed (1/t) */
void update();

/* Calls when new command arrives for client `n' */
void new_command(int n);


#endif
