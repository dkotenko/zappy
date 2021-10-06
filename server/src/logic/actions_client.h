#ifndef ACTIONS_CLIENT_H
# define ACTIONS_CLIENT_H

# define CMD_NUMBER 12

# include "zappy.h"
enum e_command
{
	CMD_AVANCE,
	CMD_DROITE,
	CMD_GAUCHE,
	CMD_VOIR,
	CMD_INVENTAIRE,
	CMD_PREND,
	CMD_POSE,
	CMD_EXPULSE,
	CMD_BROADCAST,
	CMD_INCANTATION,
	CMD_FORK,
	CMD_CONNECT_NBR
};

typedef void	(*action)(t_player *);
typedef void	(*action_arg)(t_player *, char *);

typedef struct s_cmd_aux {
	int			*duration;
	char		**name;
	action		*f;
	action_arg 	*f_arg;
	int			*req_arg;
} t_dict;

void	init_cmd();

/*
 * actions
 */
void	avanche(t_player *player);
void	droite(t_player *player); //Повернуть направо
void	gauche(t_player *player); //Повернуть налево
void	inventory(int *inv);
void	voir(t_player *player);
void	incantation(t_game *game, t_player *player);
void	expulse(t_player *player);
void	connect_nbr();
void	do_fork(t_player *player);
void	pose(t_player *player, char *data);
void	prend(t_player *player, char *data);
void	broadcast();

/*
** game actions
*/
void	starving_n_death(void);
int		is_session_end(void);
t_list *get_winners(void);


#endif