/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:05:05 by gmelisan          #+#    #+#             */
/*   Updated: 2021/10/24 14:52:44 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/select.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

#include "server.h"
#include "utils.h"
#include "logger.h"
#include "circbuf.h"
#include "commands.h"
#include "logic.h"
#include "reception.h"
#include "graphic.h"
#include "admin.h"

# define T_MAX						1000
#define MAX_PENDING_COMMANDS		10

#define CIRCBUF_SIZE				64
#define CIRCBUF_ITEM_SIZE			32

enum e_type {
	FD_FREE,
	FD_SERV,
	FD_CLIENT,
	FD_GFX
};

typedef struct s_fd {
	enum e_type type;
	void (*fct_read)(int);			/* read data if client, accept if server */
	void (*fct_write)(int);
	int (*fct_handle)(int, char *);
	t_circbuf circbuf_read;		/* raw data from client socket goes here */
	t_circbuf circbuf_write;	/* raw data to be written to client goes here */
	int pending_commands;		/* how many commands for this client in `commands' */
	t_command *last_command;	/* next pending command execution time should be
													 calculated from last command */
} t_fd;

typedef struct s_env {
	t_fd *fds;					/* each client is represented as fd */
	int maxfd;					/* max fd in system, size of fds */
	int max;					/* first argument of select (-1) */
	int r;						/* return value of select */
	fd_set fd_read;				/* second argument of select */
	fd_set fd_write;			/* third argument of select */
	struct timeval t;			/* time after select */
	struct timeval tu;			/* time unit */
	int *deadbodies;			/* array with same size of fds */
	t_circbuf circbuf_events;	/* date to be written to gfx clients */
} t_env;

t_env env;



static void clean_fd(t_fd *fd)
{
	fd->type = FD_FREE;
	fd->fct_read = NULL;
	fd->fct_write = NULL;
	fd->fct_handle = NULL;
	fd->pending_commands = 0;
	fd->last_command = NULL;
}

static void client_gone(int cs)
{
	close(cs);
	clean_fd(&env.fds[cs]);
	circbuf_clear(&env.fds[cs].circbuf_read);
	circbuf_clear(&env.fds[cs].circbuf_write);
	reception_remove_client(cs);
	/* TODO */
	/* commands_popnb(cs, MAX_PENDING_COMMANDS); */ 
	log_info("Client #%d gone away", cs);
}

static struct timeval tu2tv(int tu)
{
	struct timeval t;
	extern t_env env;

	memset(&t, 0, sizeof(t));
	for (int i = 0; i < tu; ++i)
		timeradd(&t, &env.tu, &t);
	return t;
}

static int client_handle_command(int client_id, char *command)
{
	t_fd *client = &env.fds[client_id];
	t_command *cmd;

	log_debug("#%d -> srv: '%s'", client_id, command);
	if (client->pending_commands == MAX_PENDING_COMMANDS) {
		log_warning("drop command '%s': queue is full", command);
		return 0;
	}
	
	int command_id = lgc_get_command_id(command);
	if (command_id == -1) {
		log_warning("unknown command '%s' from #%d", command, client_id);
		return 0;
	}
	
	int dur = g_cfg.cmd.duration[command_id];
	if (dur == 0) {
		cmd = command_new(env.t, g_cfg.cmd.name[command_id], client_id);
		lgc_execute_command(client_id, command, command_id);
		command_del(cmd);
		return 0;
	}
	struct timeval t = tu2tv(dur);
	if (client->pending_commands) {
		log_debug("client->pending_commands: %d", client->pending_commands);
		timeradd(&t, &client->last_command->t, &t);
	} else {
		timeradd(&t, &env.t, &t);
	}
	cmd = command_new(t, strdup(command), client_id);
	commands_push(cmd);
	client->last_command = cmd;
	++client->pending_commands;
	return 0;
}

static void	client_read(int cs)
{
	size_t r = 0;
	char buf[CIRCBUF_ITEM_SIZE] = {0};
	t_fd *client = &env.fds[cs];
	int handle_result;
	
	r = recv(cs, buf, sizeof(buf), 0);
	if (r <= 0) {
		lgc_player_gone(cs);
		client_gone(cs);
		return ;
	}
	circbuf_push(&client->circbuf_read, buf);

	if (strchr(buf, '\n') == NULL)
		return ;
	char *command = circbuf_pop_string(&env.fds[cs].circbuf_read);
	*strchr(command, '\n') = 0; // TODO this split loses part of next command if any
	handle_result = client->fct_handle(cs, command);
	free(command);
	if (client->fct_handle != reception_chat)
		return ;
	switch (handle_result) {
	case RECEPTION_ROUTE_EXIT:
		srv_client_died(cs);
		break ;
	case RECEPTION_ROUTE_CLIENT:
		client->fct_handle = client_handle_command;
		lgc_new_player(cs, reception_find_client_team(cs));
		break ;
	case RECEPTION_ROUTE_GFX:
		client->fct_handle = graphic_chat;
		client->type = FD_GFX;
		break ;
	case RECEPTION_ROUTE_ADMIN:
		client->fct_handle = admin_chat;
		break ;
	}
}

static void client_write(int cs)
{
	char *data;
	
	while (env.fds[cs].circbuf_write.len) {
		data = (char *)circbuf_pop(&env.fds[cs].circbuf_write);
		if (data[CIRCBUF_ITEM_SIZE - 1] != '\0')
			send(cs, data, CIRCBUF_ITEM_SIZE, 0);
		else
			send(cs, data, strlen(data), 0);
		free(data);
	}
}

static void check_fd()
{
	int i;
	
	for (i = 0; i <= env.maxfd && env.r > 0; ++i) {
		if (FD_ISSET(i, &env.fd_read))
			env.fds[i].fct_read(i);
		if (FD_ISSET(i, &env.fd_write))
			env.fds[i].fct_write(i);
		if (FD_ISSET(i, &env.fd_read) || FD_ISSET(i, &env.fd_write))
			--env.r;
	}
	for (i = 0; env.deadbodies[i] != 0 && i <= env.maxfd; ++i) {
		client_write(env.deadbodies[i]);
		client_gone(env.deadbodies[i]);
		env.deadbodies[i] = 0;
	}
}


/*

TC - time current
TU - time unit (1/t)
TO - timeout for select

(TC = 00:00)
0. push 0 with time TC + TU		[00:01, 0]
	TO = TU
1. r = select(..., TO)
	(TC = 00:01.5)
2. if r == 0:
	pop
	exec
	if command 0:
		push poped T+TU	[00:02, 0]
	
else
	detect new command (duration 7/t)	
	push (TC + duration)	[00:01, 0] [00:07.5, 1]

3. pop. TO = poped T-TC
4. goto 1

--------------------------------------

 Nt - n-th time unit
 a2 - command a lasts 2 time units
 ae - end of command a

 t    * *  *  * * ** *    *       - program returns from select at these times
 |---------------------------
      ^ |  ^  | ^ || ^    ^
      1t|  2t | 3t|| 4t   5t
        ^     |   ^|
       a2     ^  ae^
              b1   be
*/

static void do_select()
{
	struct timeval tc;			/* time current */
	static struct timeval to;	/* timeout */
	struct timeval t;			/* tmp */
	t_command *command;

	memset(&to, 0, sizeof(to));
	command = commands_min(); /* minimal by time, what we should execute after select */
	if (commands_is_empty())
		lgc_init();
	
	xassert(gettimeofday(&tc, NULL) != -1, "gettimeofday");
	if (commands_is_empty()) {	/* init */
		timeradd(&tc, &env.tu, &t); /* wake up from select after env.tu (1/t) */
		command = command_new(t, NULL, 0); /* command without client is lgc_update */
		commands_push(command);
		to = env.tu;
	} else {
		if (timercmp(&command->t, &tc, >)) {
			timersub(&command->t, &tc, &to);
		} else {
			memset(&to, 0, sizeof(to));
			log_warning("Out of time on command execution");
		}
	}
	env.r = select(env.max + 1, &env.fd_read, &env.fd_write, NULL, &to);
	if (env.r == -1)
		log_warning("select: %s", strerror(errno));
	xassert(gettimeofday(&env.t, NULL) != -1, "gettimeofday");
	if (env.r == 0) {			/* select woke up for command, not for read/write */
		if (command->data == NULL) {
			log_tick(&to);
			lgc_update();
			timeradd(&command->t, &env.tu, &t);
			commands_push(command_new(t, NULL, 0));
		} else {
			lgc_execute_command(command->client_nb, command->data, -1);
			--env.fds[command->client_nb].pending_commands;
		}
		commands_pop(command);
	}
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
		if (env.fds[i].type == FD_GFX) {
			circbuf_concat(&env.fds[i].circbuf_write, &env.circbuf_events);
			if ((env.fds[i].circbuf_write.len) > 0)
				FD_SET(i, &env.fd_write);
		}
	}
	circbuf_reset(&env.circbuf_events);
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
	env.fds[cs].fct_handle = reception_chat;
	env.fds[cs].circbuf_read = circbuf_init(CIRCBUF_SIZE, CIRCBUF_ITEM_SIZE);
	env.fds[cs].circbuf_write = circbuf_init(CIRCBUF_SIZE + g_cfg.width, CIRCBUF_ITEM_SIZE);

	env.fds[cs].fct_handle(cs, NULL);
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
	sin.sin_port = htons(g_cfg.port);
	xassert(bind(s, (struct sockaddr*)&sin, sizeof(sin)) != -1, "bind");
	xassert(listen(s, 42) != -1, "listen");
	log_info("Listen on port %d", g_cfg.port);
	env.fds[s].type = FD_SERV;
	env.fds[s].fct_read = srv_accept;

	if (srv_update_t(g_cfg.t) == -1)
		log_fatal("Invalid time unit (%d): "
				  "must be in range [1, %d]", g_cfg.t, T_MAX);
}

static void srv_init()
{
	struct rlimit rlp;
	
	xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
	env.maxfd = rlp.rlim_cur;
	log_info("Maximum clients: %d", env.maxfd);
	env.fds = (t_fd *)malloc(sizeof(t_fd) * env.maxfd);
	xassert(env.fds != NULL, "malloc");
	for (int i = 0; i < env.maxfd; ++i) {
		clean_fd(env.fds + i);
	}
	env.deadbodies = (int *)calloc(sizeof(int), env.maxfd);
	env.circbuf_events = circbuf_init(CIRCBUF_SIZE, CIRCBUF_ITEM_SIZE);
}

static void sigh(int n) {
	(void)n;
	srv_stop();
}

void srv_start()
{
	log_info("Start");
	signal(SIGINT, sigh);
	srv_init();
	srv_create();
	reception_init(env.maxfd);
	while (21) {
		init_fd();
		do_select();
		check_fd();
	}
}

void srv_stop()
{
	commands_destroy();
	reception_clear();
	for (int i = 0; i < env.maxfd; ++i)
		if (env.fds[i].type != FD_FREE && env.fds[i].type != FD_SERV) {
			circbuf_clear(&env.fds[i].circbuf_read);
			circbuf_clear(&env.fds[i].circbuf_write);
		}
	circbuf_clear(&env.circbuf_events);
	free(env.fds);
	free(env.deadbodies);
	clear_cmd();
	log_info("Exit");
	exit(0);
}

void srv_event(char *msg, ...)
{
	char *buf;
	va_list ap;
	
	va_start(ap, msg);
	xassert(vasprintf(&buf, msg, ap) != -1, "vasprintf");
	circbuf_push_string(&env.circbuf_events, buf);
	free(buf);
	va_end(ap);
}

void srv_reply_client(int client_nb, char *msg, ...)
{
	char *buf;
	va_list ap;

	va_start(ap, msg);
	xassert(vasprintf(&buf, msg, ap) != -1, "vasprintf");
	circbuf_push_string(&env.fds[client_nb].circbuf_write, buf);
	free(buf);
	va_end(ap);
}

void srv_flush_client(int client_nb)
{
	if ((env.fds[client_nb].circbuf_write.len) > 0)
		env.fds[client_nb].fct_write(client_nb);
}

void srv_client_died(int client_nb)
{
	int i = 0;
	
	while (env.deadbodies[i] != 0 && i <= env.maxfd)
		++i;
	env.deadbodies[i] = client_nb;
}

int srv_update_t(int t)
{
	struct timeval tmp;

	memset(&tmp, 0, sizeof(tmp));
	
	if (t == 1)
		tmp.tv_sec = 1;
	else if (t > 1 && t <= T_MAX)
		tmp.tv_usec = 1000000 / t;
	else
		return -1;
	env.tu = tmp;
	return t;
}

void srv_push_command(char *cmd, int after_t)
{
	struct timeval tc;
	struct timeval t;

	xassert(gettimeofday(&tc, NULL) != -1, "gettimeofday");
	t = tu2tv(after_t);
	timeradd(&t, &tc, &t);
	commands_push(command_new(t, cmd, 0));
}

#undef CIRCBUF_SIZE
#undef CIRCBUF_ITEM_SIZE
