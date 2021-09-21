/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:05:05 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/21 18:48:13 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/select.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "server.h"
#include "utils.h"
#include "logger.h"
#include "circbuf.h"

#define CIRCBUF_SIZE		16
#define CIRCBUF_ITEM_SIZE	32

enum e_type {
	FD_FREE,
	FD_SERV,
	FD_CLIENT
};

typedef struct s_fd {
	enum e_type type;
	void (*fct_read)();
	void (*fct_write)();
	char buf_read[CIRCBUF_ITEM_SIZE]; // push to circbuf_read when full
	size_t buf_read_pos;
	t_circbuf circbuf_read;
	t_circbuf circbuf_write;
} t_fd;

typedef struct s_env {
	t_fd *fds;
	int maxfd;
	int max;
	int r;
	fd_set fd_read;
	fd_set fd_write;
	struct timeval timeout;
} t_env;

static t_env env;

static void clean_fd(t_fd *fd)
{
	fd->type = FD_FREE;
	fd->fct_read = NULL;
	fd->fct_write = NULL;
}

static void client_gone(int cs)
{
	close(cs);
	clean_fd(&env.fds[cs]);
	circbuf_clear(&env.fds[cs].circbuf_read);
	circbuf_clear(&env.fds[cs].circbuf_write);
	log_info("Client #%d gone away", cs);
}

static void	client_read(int cs)
{
	size_t r = 0;
	char buf[CIRCBUF_ITEM_SIZE];
	//char *p_newline;
	//size_t i = 0;

	t_fd *client = &env.fds[cs];

	// hel_lowo_rld\n
	// -> hel
	// [....] (item_size = 4)
	// [hel.], pos = 3
	// -> lowo
	// [hell][....]
	// [hell][owo.]
	// -> rld\n
	// [hell][owor][ld\n.]
	

	r = recv(cs, buf, sizeof(buf), 0);
	if (r <= 0) {
		client_gone(cs);
		return ;
	}
	size_t right_part = sizeof(client->buf_read) - client->buf_read_pos;
	if (r >= right_part) {
		char *copy_to = client->buf_read + client->buf_read_pos;
		memcpy(copy_to, buf, right_part);
		circbuf_push(&client->circbuf_read, client->buf_read);
		memcpy(client->buf_read, buf + right_part, sizeof(client->buf_read) - right_part);
		client->buf_read_pos = sizeof(client->buf_read) - right_part;
	} else {
		char *copy_to = client->buf_read + client->buf_read_pos;
		memcpy(copy_to, buf, r);
		client->buf_read_pos += r;
	}

	if (strchr(buf, '\n') != NULL) {
		char *command = circbuf_pop_string(&env.fds[cs].circbuf_read);
		*strchr(command, '\n') = 0;
		log_info("got command '%s'", command);
	
		if (strcmp(command, "stop server") == 0)
			exit(0);
		//circbuf_push_string(&env.fds[cs].circbuf_write, command);
	
		free(command);
		
	}
	

	// -> lowor
	// r = 4
	//  01234
	// [hel..]
	// pos = 3
	// size = 5
	// memcpy(3, "lowor", 5-3)
	// [hello]
	// memcpy(0, "wor", 3)
	


	
	


	/*
	client->buf_read_pos += r;
	if (client->buf_read_pos >= CIRCBUF_ITEM_SIZE) {
		circbuf_push(client->circbuf_read, client->buf_read);
		client->buf_read_pos =
	}

	*/
	
	




	
/*
	while (1) {
		r = recv(cs, buf + i, sizeof(buf), 0);
		if (r <= 0) {
			client_gone(cs);
			return ;
		}
		p_newline = strchr(buf, '\n');
		if (p_newline) {
			circbuf_push(&env.fds[cs].circbuf_read, buf);
			break ;
		}
		if (i + r < sizeof(buf)) {
			i += r;
		} else {
			i = 0;
			memset(buf, 0, CIRCBUF_ITEM_SIZE);
		}
	}
*/	

	
/*
	do {
		r = recv(cs, buf, sizeof(buf), 0);
		if (r <= 0) {
			client_gone(cs);
			return ;
		}
		circbuf_push(&env.fds[cs].circbuf_read, buf);
	} while ((p_newline = strchr(buf, '\n')) == NULL);
*/

/*
	char *command = circbuf_pop_string(&env.fds[cs].circbuf_read);
	*strchr(command, '\n') = 0;
	log_info("got command '%s'", command);
	
	if (strcmp(command, "stop server") == 0)
		exit(0);
	//circbuf_push_string(&env.fds[cs].circbuf_write, command);
	
	free(command);
*/
}

static void client_write(int cs)
{
	while (env.fds[cs].circbuf_write.len) {
		send(cs, "abc\n", 3, 0);
		break ;
	}
}

static void check_fd()
{
	for (int i = 0; i < env.maxfd && env.r > 0; ++i) {
		if (FD_ISSET(i, &env.fd_read))
			env.fds[i].fct_read(i);
		if (FD_ISSET(i, &env.fd_write))
			env.fds[i].fct_write(i);
		if (FD_ISSET(i, &env.fd_read) || FD_ISSET(i, &env.fd_write))
			--env.r;
	}
}

static void do_select()
{
	env.r = select(env.max + 1, &env.fd_read, &env.fd_write, NULL, NULL);
}

static void init_fd()
{
	env.max = 0;
	FD_ZERO(&env.fd_read);
	FD_ZERO(&env.fd_write);
	for (int i = 0; i < env.maxfd; ++i) {
		if (env.fds[i].type != FD_FREE) {
			FD_SET(i, &env.fd_read);
			if ((env.fds[i].circbuf_write.len) > 0)
				FD_SET(i, &env.fd_write);
			env.max = (env.max > i ? env.max : i);
		}
	}

}

static void srv_accept(int s)
{
	int cs;
	struct sockaddr_in csin;
	socklen_t csin_len;

	csin_len = sizeof(csin);
	xassert((cs = accept(s, (struct sockaddr *)&csin, &csin_len)) != -1, "accept");
	log_info("New client #%d (%s:%d)", cs, inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
	clean_fd(&env.fds[cs]);
	env.fds[cs].type = FD_CLIENT;
	env.fds[cs].fct_read = client_read;
	env.fds[cs].fct_write = client_write;
	env.fds[cs].circbuf_read = circbuf_init(CIRCBUF_SIZE, CIRCBUF_ITEM_SIZE);
	env.fds[cs].circbuf_write = circbuf_init(CIRCBUF_SIZE, CIRCBUF_ITEM_SIZE);
}

static void srv_create()
{
	int s;
	struct sockaddr_in sin;
	struct protoent *pe;

	xassert((pe = getprotobyname("tcp")) != NULL, "getprotobyname");
	xassert((s = socket(PF_INET, SOCK_STREAM, pe->p_proto)) != -1, "socket");
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(g_main_config.port);
	xassert(bind(s, (struct sockaddr*)&sin, sizeof(sin)) != -1, "bind");
	xassert(listen(s, 42) != -1, "listen");
	log_info("Listen on port %d", g_main_config.port);
	env.fds[s].type = FD_SERV;
	env.fds[s].fct_read = srv_accept;
}

static void srv_init()
{
	struct rlimit rlp;
	
	xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
	env.maxfd = rlp.rlim_cur;
	log_debug("maxfd = %d", env.maxfd);
	env.fds = malloc(sizeof(t_fd) * env.maxfd);
	xassert(env.fds != NULL, "malloc");
	for (int i = 0; i < env.maxfd; ++i) {
		clean_fd(env.fds + i);
	}
}

static void sigh(int n) {
	(void)n;
	free(env.fds);
	
	log_info("Exit");
	exit(0);
}

void srv_start()
{
	g_circbuf_debug = 1;
	log_info("Start");
	signal(SIGINT, sigh);
	srv_init();
	srv_create();
	while (21) {
		init_fd();
		do_select();
		check_fd();
	}

}

#undef BUF_SIZE
