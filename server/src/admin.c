/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 16:35:53 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/30 17:05:44 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "server.h"
#include "admin.h"

// TODO implement hash algo or use library (it's bonus so why not)
#define ADMIN_PASSWORD	"zappy"

enum e_admin_chat_state {
	ADMIN_CHAT_STATE_PASSWORD,
	ADMIN_CHAT_STATE_DEFAULT
};

static void welcome(int client_nb)
{
	srv_reply_client(client_nb, "Welcome, admin. You can use following commands:\n");
	srv_reply_client(client_nb, "1) stop server - shutting down the server\n");
	srv_reply_client(client_nb, "2) time n - set server time divider in range [1:1000]\n");
}

int admin_chat(int client_nb, char *message)
{
	char a[3000];
	static int state = ADMIN_CHAT_STATE_PASSWORD;
	
	if (!message) {
		srv_reply_client(client_nb, "Please enter password:\n");
		return 0;
	}
	switch (state) {
	case ADMIN_CHAT_STATE_PASSWORD:
		if (strcmp(message, ADMIN_PASSWORD) == 0) {
			welcome(client_nb);
			state = ADMIN_CHAT_STATE_DEFAULT;
		} else {
			srv_reply_client(client_nb, "Wrong password, try again:\n");
		}	
		break ;
	case ADMIN_CHAT_STATE_DEFAULT:
		if (strcmp(message, "stop server") == 0)
			srv_stop();
		else if (strcmp(message, "test") == 0) {
			
			memset(a, 'a', 3000);
			a[2999] = 0;
			srv_reply_client(client_nb, "%s\n", a);
		}
		else if (strncmp(message, "time ", 5) == 0) {
			int time = atoi(message + 5);
			if ((time = srv_update_t(time)) == -1) {
				srv_reply_client(client_nb, "sbp\n");
				return 0;
			}
			srv_event("sgt %d\n", time);
		}
		else
			srv_reply_client(client_nb, "Unknown command\n");
		break ;
	}
	return 0;
}
