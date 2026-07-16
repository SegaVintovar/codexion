#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>

typedef enum	e_state
{
	COMPILE,
	REFACTOR,
	DEBUG
}	t_state;

typedef enum	e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct	s_quantum_compiler
{
	int			coders_c;
	int			burnout_t;
	int			compile_t;
	int			debug_t;
	int			refactor_t;
	int			comp_c_r;
	int			dongle_cd;
	t_scheduler	scheduler;

}	t_quantum_compiler;

typedef struct	s_dongle
{
	bool	locked;
	int		id;
}	t_dongle;

t_dongle	*dongle_new(int id);
void        dongle_unlock(t_dongle *dongle);
void        dongle_lock(t_dongle *dongle);
void        free_dongle(t_dongle *dongle);

# endif
