#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>

typedef struct s_quantum_compiler
{
	int coders_num;
}	t_quantum_compiler;

typedef struct s_dongle
{
	bool locked;
	int id;
}	t_dongle;

# endif