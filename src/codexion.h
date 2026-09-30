/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   codexion.h										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: vs <vs@student.42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/09/05 15:12:47 by vsudak			#+#	#+#			 */
/*   Updated: 2026/09/29 14:03:04 by vs			   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>
# include <stdint.h>
// # include <stdatomic.h>

typedef struct s_quantum_compiler	t_quantum_compiler;
typedef struct s_coder				t_coder;

typedef enum e_scheduler
{
	FIFO,
	EDF,
	NONE
}	t_scheduler;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	uint64_t			avaliable_at;
	int					id;
	int					buzy;
	t_coder				**queue;
	pthread_mutex_t		queue_mutex;
}	t_dongle;

typedef struct s_coder
{
	pthread_t			thread;
	int					id;
	t_dongle			*left;
	t_dongle			*right;
	uint64_t			last_comp_t;
	int					compiles_left;
	t_quantum_compiler	*state;
	pthread_cond_t		stop_cond;
	pthread_mutex_t		time_check;
}	t_coder;

typedef struct s_quantum_compiler
{
	int				coders_c;
	int				burnout_t;
	int				compile_t;
	int				debug_t;
	int				refactor_t;
	int				comp_c_r;
	int				dongle_cd;
	t_scheduler		scheduler;
	t_dongle		**dongles;
	t_coder			**coders;
	uint64_t		start_time;
	pthread_cond_t	burnout_sig;
	pthread_t		monitor_thread;
	int				burnout_reported;
	pthread_mutex_t	burnout_mutex;
	int				who_got_burned;
	pthread_mutex_t	last_comp_t_mutex;	
	uint64_t		when_we_got_burned;
	int				coders_finished;
	pthread_mutex_t	print_m;
}	t_quantum_compiler;

// time / utils
int					isint(char *arg);
int					ft_isdigit(int c);
long				my_atoi(const char *nptr);
uint64_t			curtime_full(void);
uint64_t			time_scince_start(t_quantum_compiler *state);
uint64_t			converter(uint64_t t);
uint64_t			deadline_of(t_coder *coder);
uint64_t			cap_to_deadline(t_coder *coder, uint64_t want);
int					claim_dongle(
						t_dongle *dongle, t_coder *coder, uint64_t *avail_at);
void				release_dongle(t_dongle *dongle, uint64_t avail_at);
void				set_the_time(t_quantum_compiler *state);
void				set_last_comp_time(t_coder *coder);

// input_check
int					is_scheldue(char *arg);
int					input_check(int argc, char **argv);
t_scheduler			what_is_our_scheldue(char *arg);
int					start_batch_of_coders(t_quantum_compiler *state, int batch);
void				join_first_batch(t_quantum_compiler *state);

// quantum compiler, dongle, coder methods
t_quantum_compiler	*init_compiler(int argc, char **argv);
t_dongle			**init_dongles(t_quantum_compiler *instance);
t_coder				*new_coder(int id, t_quantum_compiler *state);
void				assign_dongles(t_coder *coder, t_quantum_compiler *state);
t_coder				**init_coders(t_quantum_compiler *state);
t_dongle			*dongle_new(int id);
void				free_dongle(t_dongle *dongle);
int					dongle_acquisition(t_coder *coder);
void				drop_dongles(t_coder *coder);
int					grab_dongle(t_dongle *first, t_coder *coder);

// simulation
void				*sim(void *coder);
void				run(t_quantum_compiler *state);
int					new_comp(t_coder *coder);
int					refactoring(t_coder *coder, t_quantum_compiler *state);
int					debugging(t_coder *coder, t_quantum_compiler *state);
void				coder_finished(t_quantum_compiler *state);

// monitor
int					start_monitor(t_quantum_compiler *state);
void				stop_monitor(t_quantum_compiler *state);

// use it to print without datarace
void				safe_print(
						t_quantum_compiler *state,
						t_coder *coder,
						char *stage);
// here we are checking if someone esle has already reported about Burnout
int					burnout_rep_check(t_quantum_compiler *state);
// these functions have same purpose = they are checking for
//  the burnout of the current coder
int					burnoutCheck(t_quantum_compiler *state, t_coder *coder);
int					is_burned(t_quantum_compiler *state, t_coder *coder);
void				burnout_report(
						t_quantum_compiler *state,
						t_coder *coder,
						uint64_t when);
// int					will_b_burned(t_coder *coder, uint64_t next_stop);
// queue
t_coder				**init_queue(void);
void				destroy_queue(t_coder **queue);
void				enque(t_dongle *dongle, t_coder *coder);
void				pop(t_dongle *dongle);

#endif
