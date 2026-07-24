#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>


typedef struct s_dongle {
    pthread_mutex_t		mutex;
    uint64_t			time;
    int                 id;
}   t_dongle;

typedef struct s_coder
{
    pthread_t   coder;
    // t_state     state;
    t_dongle    *left;
    t_dongle    *right;
    uint64_t    t_since_lc;
}   t_coder;

typedef struct	s_quantum_compiler
{
	int			coders_c;
	int			burnout_t;
	int			compile_t;
	int			debug_t;
	int			refactor_t;
	int			comp_c_r;
	int			dongle_cd;
	// t_scheduler	scheduler;
    t_dongle    **dongles;
    t_coder     *coders;
}	t_quantum_compiler;


// struct timeval {
// 	time_t      tv_sec;     /* seconds */
// 	suseconds_t tv_usec;    /* microseconds */
// };

// I need a struct to pass it to the working func

void *worker(void *arg)
{
	// printf("Doing a job\n");
	struct timeval		curtime;
	uint64_t			curtime_full;
	t_dongle			*dongle;
	pthread_mutex_t		*mutex;

	gettimeofday(&curtime, NULL);
	curtime_full = (curtime.tv_sec * (uint64_t)1000) + (curtime.tv_usec / 1000);
	// i need to change argument to quantum_compiler
    // so i can get more data
    mutex = (pthread_mutex_t *)arg;
	// mutex = dongle->mutex;
	// usleep(1000000);
	pthread_mutex_lock(mutex);
	
	printf("Since Unix Epoch(miliseconds): %ld\n", ((long)curtime_full));
	long years = (long)curtime_full / 31556952000;
	printf("Since Unix Epoh(years): %ld\n", years);
	// dongle->time = curtime_full;
	pthread_mutex_unlock(mutex);
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t	*thread;
    t_coder     *coders;
	int			thread_ret_val;
	int			join_ret_val;
	int			amount_of_coders;
	int			i;
    int         amount_of_compilations;
	pthread_mutex_t     mutex;
    t_quantum_compiler  *state;

    state = malloc(sizeof(t_quantum_compiler));
    // malloc check
	pthread_mutex_init(&mutex, NULL);
	amount_of_coders = atoi(argv[1]);
    amount_of_compilations = atoi(argv[2]);
    state->coders_c = amount_of_coders;
    state->comp_c_r = amount_of_compilations;
	coders = malloc(sizeof(t_coder) * amount_of_coders);
    // give dongles to the coders
    state->coders = coders;
    // i = 0;
    // while (i < amount_of_coders)
    // {
    //     coders[i]->coder = malloc(sizeof(pthread_t));
    //     i++; 
    // }
	// I think this part goes under another loop that counts amount of the compilations
    while (amount_of_compilations)
    {
        i = 0;
        while (i < amount_of_coders)
		{
            pthread_create(&coders[i++].coder, NULL, worker, (void *)&mutex);
		}
		i = 0;
        while (i < amount_of_coders)
            pthread_join(coders[i++].coder, NULL);
        amount_of_compilations--;
    }
	// 
	pthread_mutex_destroy(&mutex);
	if (coders)
		free(coders);
	return 0;
}
