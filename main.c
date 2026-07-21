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
}   t_dongle;

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
	mutex = (pthread_mutex_t *)arg;
	// mutex = dongle->mutex;
	usleep(1000000);
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
	int			thread_ret_val;
	int			join_ret_val;
	int			amount_of_coders;
	int			i;
    int         amount_of_compilations;
	pthread_mutex_t mutex;

	pthread_mutex_init(&mutex, NULL);
	amount_of_coders = atoi(argv[1]);
    amount_of_compilations = atoi(argv[2]);
	thread = malloc(sizeof(pthread_t) * amount_of_coders);
	// I think this part goes under another loop that counts amount of the compilations
    while (amount_of_compilations)
    {
        i = 0;
        while (i < amount_of_coders)
		{
            pthread_create(&thread[i++], NULL, worker, (void *)&mutex);
		}
		i = 0;
        while (i < amount_of_coders)
            pthread_join(thread[i++], NULL);
        amount_of_compilations--;
    }
	// 
	pthread_mutex_destroy(&mutex);
	// printf("%d\n", counter);
	if (thread)
		free(thread);
	return 0;
}
