/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:05:05 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/24 16:40:52 by gmelisan         ###   ########.fr       */
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
	char buf[CIRCBUF_ITEM_SIZE] = {0};
	t_fd *client = &env.fds[cs];
	
	r = recv(cs, buf, sizeof(buf), 0);
	if (r <= 0) {
		client_gone(cs);
		return ;
	}
	circbuf_push(&client->circbuf_read, buf);

	if (strchr(buf, '\n') != NULL) {
		char *command = circbuf_pop_string(&env.fds[cs].circbuf_read);
		*strchr(command, '\n') = 0;
		log_info("got command '%s'", command);
	
		if (strcmp(command, "stop server") == 0)
			exit(0);
		circbuf_push_string(&env.fds[cs].circbuf_write, "You wrote:\n");
		circbuf_push_string(&env.fds[cs].circbuf_write, command);
		circbuf_push_string(&env.fds[cs].circbuf_write, "\n");
	
		free(command);
		
	}
	
}

static void client_write(int cs)
{
	void *data;
	
	while (env.fds[cs].circbuf_write.len) {
		data = circbuf_pop(&env.fds[cs].circbuf_write);
		send(cs, data, CIRCBUF_ITEM_SIZE, 0);
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

#undef CIRCBUF_SIZE
#undef CIRCBUF_ITEM_SIZE