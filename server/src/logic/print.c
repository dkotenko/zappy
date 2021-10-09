#include "zappy.h"

extern t_game *game;

void print_map(t_game *game, int (*f)(t_cell *))
{
	int  i;
	int  j;

     i = 0;
     printf("      ");
     while (++i < game->map->w + 1)
     {
          printf("%d  ", i);
          
     }
     printf("\n\n");
     i = 0;
     while (++i < game->map->h + 1)
     {
          j = 0;
          printf("%3d   ", i);
          while (++j < game->map->w + 1)
          {
			   
               if (f(game->map->cells[i][j]))
               {
                    printf("%s%c  %s", ANSI_COLOR_RED, game->aux->orient_chars[((t_player *)(game->map->cells[i][j]->visitors->content))->orient], ANSI_COLOR_RESET);
               }
               else
               {
                    printf("%d  ", game->map->cells[i][j]->visitors_num);
               }
          }
          printf("\n");
     }
     printf("\n");
}

void	print_inverntory(int *inv)
{
	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		printf("%s: %d |", game->aux->resources[i], inv[i]);
	}
	printf('\n');
}

void	print_visitors(t_list *visitors)
{
	printf("%s", visitors ? "visitors: " : "no visitors");
	while (visitors) {
		printf("%d ", ((t_player *)visitors->content)->id);
		visitors = visitors->next;
	}
	printf("\n");
}

void	print_cell(t_cell *cell)
{
	printf("x: %d | y: %d |\n");
	print_inverntory(cell->inventory);
	printf("visitors_num: %d\n", cell->visitors_num);
	print_visitors(cell->visitors);
}

void	print_player(t_player *player)
{
	printf("id: %d | level: %d | orient: %d |\n", player->id, player->level, player->orient);
	
}

