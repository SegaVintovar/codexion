#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>

typedef struct s_quantum_compiler
{
	int coders_num;
}	t_quantum_compiler;

typedef struct s_dongle
{
	bool locked;
	int id;
}	t_dongle;

t_dongle	*dongle_new(int id);
void        dongle_unlock(t_dongle *dongle);
void        dongle_lock(t_dongle *dongle);
void        free_dongle(t_dongle *dongle);

# endif