// #ifndef CODEXION_H
// # define CODEXION_H

// # include <pthread.h>
// # include <stdio.h>
// # include <stdbool.h>
// # include <stdlib.h>
// # include <time.h>
// # include <unistd.h>
// # include <string.h>
// # include <limits.h>



// typedef enum	e_state
// {
// 	COMPILE,
// 	REFACTOR,
// 	DEBUG
// }	t_state;

// typedef enum	e_scheduler
// {
// 	FIFO,
// 	EDF
// }	t_scheduler;


// typedef struct	s_dongle
// {
// 	bool	locked;
// 	int		id;
//     pthread_mutex_t mutex;
// }	t_dongle;


// typedef struct s_coder
// {
//     pthread_t   coder;
//     t_state     state;
//     t_dongle    *left;
//     t_dongle    *right;
    
// }   t_coder;

// typedef struct  s_queue_node
// {
//     t_coder                *coders;
//     struct s_queue_node    *next;
//     struct s_queue_node    *last;
//     struct s_queue_node    *head;
// }   t_queue_node;

// typedef struct	s_queue
// {
// 	t_coder			*coder;
// }	t_queue;

// typedef struct	s_quantum_compiler
// {
// 	int			coders_c;
// 	int			burnout_t;
// 	int			compile_t;
// 	int			debug_t;
// 	int			refactor_t;
// 	int			comp_c_r;
// 	int			dongle_cd;
// 	t_scheduler	scheduler;
//     t_dongle    **dongles;
//     pthread_t   **coders;
// }	t_quantum_compiler;

// // utils
// int ft_isdigit(int c);



// t_dongle	*dongle_new(int id);
// void        dongle_unlock(t_dongle *dongle);
// void        dongle_lock(t_dongle *dongle);
// void        free_dongle(t_dongle *dongle);


// // quantum compiler methods
// t_quantum_compiler	*init_compiler(int argc, char **argv);
// void	start(t_quantum_compiler *instance);

// # endif


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


// typedef enum	e_state
// {
// 	COMPILE,
// 	REFACTOR,
// 	DEBUG
// }	t_state;

typedef struct s_quantum_compiler t_quantum_compiler;  // forward declaration
typedef struct s_monitor t_monitor;

typedef enum	e_scheduler
{
	FIFO,
	EDF,
	NONE
}	t_scheduler;


typedef struct	s_dongle {
    pthread_mutex_t		mutex;
    uint64_t			avaliable_at;
    int                 id;
}   t_dongle;


typedef struct s_coder
{
    pthread_t   thread;
    int         id;
    t_dongle    *left;
    t_dongle    *right;
    uint64_t	last_comp_t;
	int			compiles_left;
    t_quantum_compiler  *state;
	pthread_cond_t	stop_cond;
}   t_coder;


// for later
typedef struct  s_queue_node
{
    t_coder                *coders;
    struct s_queue_node    *next;
    struct s_queue_node    *last;
    struct s_queue_node    *head;
}   t_queue_node;

typedef struct	s_queue
{
	t_coder			*coder;
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
    pthread_cond_t  *burnoutSignal;
    pthread_t       monitor_tread;
}	t_quantum_compiler;

typedef struct s_monitor
{
    /* data */
    pthread_t   mon_tr;

} t_monitor;


// utils
int		isint(char *arg);
int 	ft_isdigit(int c);
long	my_atoi(const char *nptr);
uint64_t    curtime_full();

// quantum compiler, dongle, coder methods
t_quantum_compiler	*init_compiler(int argc, char **argv);
t_dongle    **init_dongles(t_quantum_compiler *instance);
t_coder 	*new_coder(int id, t_quantum_compiler *state);
void    	assign_dongles(t_coder *coder, t_quantum_compiler *state);
t_coder 	**init_coders(t_quantum_compiler *state);
t_dongle	*dongle_new(int id);
void        dongle_unlock(t_dongle *dongle, int cd_time);
void		dongle_lock(pthread_mutex_t *dongle, int coder_id, uint64_t start_time);
void        free_dongle(t_dongle *dongle);

// simulation
void    *simulation(void *args);
void    run(t_quantum_compiler *state);

// monitor
void	start_monitor(t_quantum_compiler *state);
void    stop_monitor(t_quantum_compiler *state);

# endif
