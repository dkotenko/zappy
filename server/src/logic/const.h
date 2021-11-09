#ifndef CONST_H
# define CONST_H

# define INIT_HP 1260
# define ADD_HP 126
# define RESOURCES_NUMBER 7
# define INCANTATIONS_NUMBER 7
# define RESOURCES_NUMBER_OF_PLAYERS 0
# define PLAYER_MAX_LEVEL 8

enum e_richness
{
	R_POOR = 4,
	R_MEDIUM = 16,
	R_RICH = 32
};

# define R_LEVEL R_MEDIUM

enum e_resources
{
    NOURRITURE,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
};

enum e_orientation
{
    ORIENT_N = 1,
    ORIENT_E,
    ORIENT_S,
    ORIENT_W
};

enum e_incantation
{
    L12,
    L23,
    L34,
    L45,
	L56,
	L67,
	L78
};

#endif