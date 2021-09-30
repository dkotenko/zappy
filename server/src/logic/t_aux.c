#include "zappy.h"



void	init_incantation_levels(int **inc)
{
	inc[L12][RESOURCES_NUMBER_OF_PLAYERS] = 1;
	inc[L12][LINEMATE] = 1;
	inc[L12][DERAUMERE] = 0;
	inc[L12][SIBUR] = 0;
	inc[L12][MENDIANE] = 0; 
	inc[L12][PHIRAS] = 0;
	inc[L12][THYSTAME] = 0;

	inc[L23][RESOURCES_NUMBER_OF_PLAYERS] = 2;
	inc[L23][LINEMATE] = 1;
	inc[L23][DERAUMERE] = 1;
	inc[L23][SIBUR] = 1;
	inc[L23][MENDIANE] = 0; 
	inc[L23][PHIRAS] = 0;
	inc[L23][THYSTAME] = 0;

	inc[L34][RESOURCES_NUMBER_OF_PLAYERS] = 2;
	inc[L34][LINEMATE] = 2;
	inc[L34][DERAUMERE] = 0;
	inc[L34][SIBUR] = 1;
	inc[L34][MENDIANE] = 0; 
	inc[L34][PHIRAS] = 2;
	inc[L34][THYSTAME] = 0;

	inc[L45][RESOURCES_NUMBER_OF_PLAYERS] = 4;
	inc[L45][LINEMATE] = 1;
	inc[L45][DERAUMERE] = 1;
	inc[L45][SIBUR] = 2;
	inc[L45][MENDIANE] = 0; 
	inc[L45][PHIRAS] = 1;
	inc[L45][THYSTAME] = 0;

	inc[L56][RESOURCES_NUMBER_OF_PLAYERS] = 4;
	inc[L56][LINEMATE] = 1;
	inc[L56][DERAUMERE] = 2;
	inc[L56][SIBUR] = 1;
	inc[L56][MENDIANE] = 3; 
	inc[L56][PHIRAS] = 0;
	inc[L56][THYSTAME] = 0;

	inc[L67][RESOURCES_NUMBER_OF_PLAYERS] = 4;
	inc[L67][LINEMATE] = 1;
	inc[L67][DERAUMERE] = 2;
	inc[L67][SIBUR] = 3;
	inc[L67][MENDIANE] = 0; 
	inc[L67][PHIRAS] = 1;
	inc[L67][THYSTAME] = 0;

	inc[L78][RESOURCES_NUMBER_OF_PLAYERS] = 6;
	inc[L78][LINEMATE] = 2;
	inc[L78][DERAUMERE] = 2;
	inc[L78][SIBUR] = 2;
	inc[L78][MENDIANE] = 2; 
	inc[L78][PHIRAS] = 2;
	inc[L78][THYSTAME] = 1;
}

t_aux     *create_aux()
{
    t_aux     *new;

	new = (t_aux *)ft_memalloc(sizeof(t_aux));

	new->resources = (char **)ft_memalloc(sizeof(char *) * RESOURCES_NUMBER);
	new->resources[0] = strdup("NOURRITURE");
	new->resources[1] = strdup("LINEMATE");
	new->resources[2] = strdup("DERAUMERE");
	new->resources[3] = strdup("SIBUR");
	new->resources[4] = strdup("MENDIANE");
	new->resources[5] = strdup("PHIRAS");
	new->resources[6] = strdup("THYSTAME");

	new->len_resources = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	for (int i = 0; i < RESOURCES_NUMBER; i++) {
		new->len_resources[i] = strlen(new->resources[i]);
	}

	new->orientation = (int *)ft_memalloc(sizeof(int) * 4);
	for (int i = 0; i < 4; i++) {
		new->orientation[i] = i;
	}

	new->orient_chars = (int *)ft_memalloc(sizeof(int) * 4);
	new->orient_chars[0] = 'N';
	new->orient_chars[1] = 'E';
	new->orient_chars[2] = 'S';
	new->orient_chars[3] = 'O';

	new->incantation = (int **)ft_memalloc(sizeof(int *) * (INCANTATIONS_NUMBER));
	for (int i = 0; i < INCANTATIONS_NUMBER; i++) {
		new->incantation[i] = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	}
	init_incantation_levels(new->incantation);

	new->incantation_sum = (int *)ft_memalloc(sizeof(int) * RESOURCES_NUMBER);
	new->incantation_sum[RESOURCES_NUMBER_OF_PLAYERS] = new->incantation[L78][RESOURCES_NUMBER_OF_PLAYERS];
	for (int i = 1; i < RESOURCES_NUMBER; i++) {
		int sum = 0;
		for (int j = 0; j < RESOURCES_NUMBER; j++) {
			sum += new->incantation[j][i];
		}
		new->incantation_sum[i] = sum;
	}
	return (new);
}