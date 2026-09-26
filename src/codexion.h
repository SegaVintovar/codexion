/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   codexion.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 15:12:47 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/26 15:28:15 by vsudak        ########   odam.nl         */
/*                                                                            */
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


typedef struct s_quantum_compiler t_quantum_compiler;  // forward declaration
typedef struct s_monitor t_monitor;
typedef struct s_coder t_coder;


typedef enum	e_scheduler
{
	FIFO,
	EDF,
	NONE
}	t_scheduler;


typedef struct	s_dongle {
    pthread_mutex_t	mutex;
    uint64_t		avaliable_at;
    int             id;
    int             buzy;
	t_coder         **queue;
	pthread_mutex_t	queue_mutex;
}   t_dongle;


typedef struct s_coder
{
    pthread_t   		thread;
    int         		id;
    t_dongle    		*left;
    t_dongle    		*right;
    uint64_t			last_comp_t;
	int					compiles_left;
    t_quantum_compiler  *state;
	// never used - delete?
	pthread_cond_t		stop_cond;
	
	pthread_mutex_t		time_check;
}   t_coder;


// for scheldue or I dont need it
// I have to implement priority queue
typedef struct	s_queue
{
	t_coder			**coder;
	int				size; // or always keep it as size 2?
}	t_queue;


typedef struct	s_quantum_compiler
{
	int			    coders_c;
	int			    burnout_t;
	int			    compile_t;
	int			    debug_t;
	int			    refactor_t;
	int			    comp_c_r;
	int			    dongle_cd;
	t_scheduler	    scheduler;
    t_dongle        **dongles;
    t_coder   	    **coders;
    uint64_t        start_time;
	
    pthread_cond_t  burnoutSignal;
    pthread_t       monitor_thread;
	// int				should_stop;
	int				burnoutReported; // atomic int didnt help
	// burnout stuff
	pthread_mutex_t	burnoutMutex;
	int				whoGotBurned;

    // mutex for last comp_t, but it is here so only one thread at a time can check it
    pthread_mutex_t last_comp_t_mutex;
    
	uint64_t		whenWeGotBurn;
	int				codersFinished;

    // pthread_mutex_t state_mutex;
	// to print safely
	pthread_mutex_t	print_m;
}	t_quantum_compiler;


// time / utils
int			isint(char *arg);
int 		ft_isdigit(int c);
long		my_atoi(const char *nptr);
uint64_t    curtime_full();
uint64_t    time_scince_start(t_quantum_compiler *state);
uint64_t    converter(uint64_t t);
void		set_the_time(t_quantum_compiler *state);
void		set_last_comp_time(t_coder *coder);

// input_check
int is_scheldue(char *arg);
int input_check(int argc, char **argv);


// quantum compiler, dongle, coder methods
t_quantum_compiler	*init_compiler(int argc, char **argv);
t_dongle    **init_dongles(t_quantum_compiler *instance);
t_coder 	*new_coder(int id, t_quantum_compiler *state);
void    	assign_dongles(t_coder *coder, t_quantum_compiler *state);
t_coder 	**init_coders(t_quantum_compiler *state);
t_dongle	*dongle_new(int id); 
void        dongle_unlock(t_dongle *dongle);
// void		dongle_lock(t_dongle *dongle, t_coder *coder);
void        free_dongle(t_dongle *dongle);
int			dongleAcquisition(t_coder *coder);
void		dropDongles(t_coder *coder);
int			grabDOngle(t_dongle *first, t_coder *coder);

// simulation
void    	*simulation(void *args);
void    	run(t_quantum_compiler *state);
uint64_t	sleep_cd(t_dongle *dongle, t_coder *coder, uint64_t now);
int			new_comp(t_coder *coder);
int    		refactoring(t_coder *coder, t_quantum_compiler *state);
int    		debugging(t_coder *coder, t_quantum_compiler *state);
void		coderFinished(t_quantum_compiler *state);

// monitor
int			start_monitor(t_quantum_compiler *state);
void    	stop_monitor(t_quantum_compiler *state);

// use it to print without datarace
void		safePrint(t_quantum_compiler *state, t_coder *coder, char *stage);
// here we are checking if soeone esle has already reported about Burnout
int     	burnoutReportCheck(t_quantum_compiler *state);
// if burnout has happend then we are using this fn to send a signal to the monitor thread
void    	burnoutReport(t_quantum_compiler *state, t_coder *coder, uint64_t marge);
int			willBeBurned(t_coder *coder, t_dongle *dongle);
// these functions have same purpose = they are checking for the burnout of the current coder
int     	burnoutCheck(t_quantum_compiler *state, t_coder *coder);
int			isBurned(t_quantum_compiler *state, t_coder *coder);
void    	burnoutReport(t_quantum_compiler *state, t_coder *coder, uint64_t when);


// queue
t_coder **initQueue();
void    destroyQueue(t_coder **queue);
void    enque(t_dongle *dongle, t_coder *coder);
void    pop(t_dongle *dongle);

# endif
