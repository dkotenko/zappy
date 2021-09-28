/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:05:05 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/28 14:04:59 by gmelisan         ###   ########.fr       */
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
#include <sys/time.h>
#include <errno.h>

#include "server.h"
#include "utils.h"
#include "logger.h"
#include "circbuf.h"
#include "commands.h"
#include "logic.h"

#define MAX_PENDING_COMMANDS		10

#define CIRCBUF_SIZE				16
#define CIRCBUF_ITEM_SIZE			32

enum e_type {
	FD_FREE,
	FD_SERV,
	FD_CLIENT
};

typedef struct s_fd {
	enum e_type type;
	void (*fct_read)();			/* read data if client, accept if server */
	void (*fct_write)();
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
} t_env;

t_env env;

static void clean_fd(t_fd *fd)
{
	fd->type = FD_FREE;
	fd->fct_read = NULL;
	fd->fct_write = NULL;
	fd->pending_commands = 0;
	fd->last_command = NULL;
}

static void client_gone(int cs)
{
	close(cs);
	clean_fd(&env.fds[cs]);
	circbuf_clear(&env.fds[cs].circbuf_read);
	circbuf_clear(&env.fds[cs].circbuf_write);
	log_info("Client #%d gone away", cs);
}

struct timeval tu2tv(int tu)
{
	struct timeval t;
	extern t_env env;

	memset(&t, 0, sizeof(t));
	for (int i = 0; i < tu; ++i)
		timeradd(&t, &env.tu, &t);
	return t;
}


static void	client_read(int cs)
{
	size_t r = 0;
	char buf[CIRCBUF_ITEM_SIZE] = {0};
	t_fd *client = &env.fds[cs];
	t_command *cmd;
	
	r = recv(cs, buf, sizeof(buf), 0);
	if (r <= 0) {
		lgc_client_gone(cs);
		client_gone(cs);
		return ;
	}
	circbuf_push(&client->circbuf_read, buf);

	if (strchr(buf, '\n') == NULL)
		return ;
	char *command = circbuf_pop_string(&env.fds[cs].circbuf_read);
	*strchr(command, '\n') = 0; // TODO this split loses part of next command if any
	if (client->pending_commands == MAX_PENDING_COMMANDS) {
		log_warning("drop command '%s': queue is full", command);
		free(command);
		return ;
	}
	
	int dur = lgc_get_command_duration(command);
	if (dur == -1) {
		log_warning("unknown command '%s'", command);
		free(command);
		return ;
	}
	log_info("got command '%s'", command);
	if (dur == 0) {
		cmd = command_new(env.t, command, cs);
		lgc_execute_command(cmd);
		command_del(cmd);
		return ;
	}
	struct timeval t = tu2tv(dur);
	if (client->pending_commands) {
		log_debug("client->pending_commands: %d", client->pending_commands);
		timeradd(&t, &client->last_command->t, &t);
	} else {
		timeradd(&t, &env.t, &t);
	}
	cmd = command_new(t, command, cs);
	commands_push(cmd);
	client->last_command = cmd;
	++client->pending_commands;
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
			lgc_execute_command(command);
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

	if (g_main_config.t == 1)
		env.tu.tv_sec = 1;
	else if (g_main_config.t > 1 && g_main_config.t <= 1000000)
		env.tu.tv_usec = 1000000 / g_main_config.t;
	else
		log_fatal("Invalid time unit: %d", g_main_config.t);
	timerprint(log_debug, &env.tu, "tu");
}

static void srv_init()
{
	struct rlimit rlp;
	
	xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
	env.maxfd = rlp.rlim_cur;
	log_debug("maxfd = %d", env.maxfd);
	env.fds = (t_fd *)malloc(sizeof(t_fd) * env.maxfd);
	xassert(env.fds != NULL, "malloc");
	for (int i = 0; i < env.maxfd; ++i) {
		clean_fd(env.fds + i);
	}
	env.deadbodies = (int *)calloc(sizeof(int), env.maxfd);	
}

static void sigh(int n) {
	(void)n;
	free(env.fds);
	free(env.deadbodies);
	commands_destroy();
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

void srv_reply_client(int client_nb, char *msg)
{
	circbuf_push_string(&env.fds[client_nb].circbuf_write, msg);
}

void srv_client_died(int client_nb)
{
	int i = 0;
	
	circbuf_push_string(&env.fds[client_nb].circbuf_write, "mort\n");
	while (env.deadbodies[i] != 0 && i <= env.maxfd)
		++i;
	env.deadbodies[i] = client_nb;
}

#undef CIRCBUF_SIZE
#undef CIRCBUF_ITEM_SIZE
