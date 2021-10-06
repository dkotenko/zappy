#include "zappy.h"

int is_session_end(void)
{
	int		i;

	i = -1;
	while (++i < config->team_number)
	{
		if (teams[i]->max_level_count >=6)
		{
			return true;
		}
	}
	return false;
}

t_list *get_winners(void)
{
	int		i;
	t_list	*list = NULL;

	i = -1;
	while (++i < config->team_number)
	{
		if (teams[i]->max_level_count >=6)
		{
			ft_lstadd_pointer(list, teams[i]);
		}
	}
	return list;
}

