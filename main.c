#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_dongle {
    pthread_mutex_t mutex;
    time_t          time;
}   t_dongle;

void *worker(void *arg)
{
	// printf("Doing a job\n");
	t_dongle	*dongle;

	dongle = (t_dongle *)arg;
	pthread_mutex_lock(dongle->mutex);
	dongle->time = gettimeofday();
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
            pthread_create(&thread[i++], NULL, worker, (void *)&mutex);
        i = 0;
        while (i < amount_of_coders)
            pthread_join(thread[i++], NULL);
        amount_of_compilations--;
    }
	// 
	pthread_mutex_destroy(&mutex);
	printf("%d\n", counter);
	if (thread)
		free(thread);
	return 0;
}
