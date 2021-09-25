/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 19:05:05 by gmelisan          #+#    #+#             */
/*   Updated: 2021/09/25 22:01:37 by gmelisan         ###   ########.fr       */
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

#include "server.h"
#include "utils.h"
#include "logger.h"
#include "circbuf.h"
#include "queue.h"
#include "commands.h"
#include "logic.h"

#define CIRCBUF_SIZE				16
#define CIRCBUF_ITEM_SIZE			32
#define COMMANDS_QUEUE_SIZE			10
#define COMMANDS_QUEUE_ITEM_SIZE	1024

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
	struct timeval t;			/* time after select */
	struct timeval tu;			/* time unit */
} t_env;

t_env env;

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
	
	r = recv(cs, buf, sizeof(buf), 0);
	if (r <= 0) {
		client_gone(cs);
		return ;
	}
	circbuf_push(&client->circbuf_read, buf);

	if (strchr(buf, '\n') != NULL) {
		char *command = circbuf_pop_string(&env.fds[cs].circbuf_read);
		*strchr(command, '\n') = 0; // TODO this split loses part of next command if any
		
		int dur = lgc_get_command_duration(command);
		if (dur == -1) {
			log_warning("unknown command '%s'", command);
			free(command);
			return ;
		}
		log_info("got command '%s'", command);
		if (dur == 0) {
			t_command *cmd = command_new(env.t, command, cs);
			lgc_execute_command(cmd);
			command_del(cmd);
			return ;
		}
		struct timeval t = tu2tv(dur);
		timeradd(&t, &env.t, &t);
		commands_push(command_new(t, command, cs));
	
		if (strcmp(command, "stop server") == 0)
			exit(0);
	
		//free(command);
		
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


/* 

tu: 1/2 = 0, 500'000 us
start: 12345, 000000 us

select(..., 0; 500'000)
r: returned at 12345, 200'000 us

-> new_command()

a = r - start = 200'000 us - how long select waited
b = tu - a = 300'000 us - how long need to wait on next select

select(..., b)
r: returned at 12345, 500,001 us

a = r - start = 500'001 - how long select waited
a > tu

-> update()


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


 */




static void do_select()
{
	struct timeval tc;
	static struct timeval to;
	struct timeval t;
	t_command *command;

	xassert(gettimeofday(&tc, NULL) != -1, "gettimeofday");
	//timerprint(log_debug, &tc, "gettimeofday");
	
	if (commands_empty()) {
		log_debug("commands_empty");
		timeradd(&tc, &env.tu, &t);
		commands_push(command_new(t, NULL, 0));
		to = env.tu;
	} else {
		command = commands_min();
		timersub(&command->t, &tc, &to);
	}
	//timerprint(log_debug, &to, "select, to");
	env.r = select(env.max + 1, &env.fd_read, &env.fd_write, NULL, &to);
	xassert(gettimeofday(&tc, NULL) != -1, "gettimeofday");
	env.t = tc;
	//timerprint(log_debug, &tc, "gettimeofday");
	command = commands_min();
	if (env.r == 0) {
		if (command->data == NULL) {
			lgc_update();
			timeradd(&command->t, &env.tu, &t);
			commands_push(command_new(t, NULL, 0));						  
		} else {
			lgc_execute_command(command);
		}
		commands_pop(command);
	}
	
}


// Nt - n-th time unit
// a2 - command a lasts 2 time units
// ae - end of command a
//
// t    * *  *  * * ** *    *       - program returns from select at these times
// |---------------------------
//      ^ |  ^  | ^ || ^    ^
//      1t|  2t | 3t|| 4t   5t
//        ^     |   ^|
//       a2     ^  ae^
//              b1   be



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

void srv_reply_client(int client_nb, char *msg)
{
	circbuf_push_string(&env.fds[client_nb].circbuf_write, msg);
}

#undef CIRCBUF_SIZE
#undef CIRCBUF_ITEM_SIZE
