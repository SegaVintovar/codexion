#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int counter = 0;

void *worker(void *arg)
{
	// printf("Doing a job\n");
	pthread_mutex_t	*mutex;

	mutex = (pthread_mutex_t *)arg;
	pthread_mutex_lock(mutex);
	counter++;
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
	pthread_mutex_t mutex;

	pthread_mutex_init(&mutex, NULL);
	amount_of_coders = atoi(argv[1]);
	thread = malloc(sizeof(pthread_t) * amount_of_coders);
	// I think this part goes under another loop that counts amount of the compilations
	i = 0;
	while (i < amount_of_coders)
		pthread_create(&thread[i++], NULL, worker, (void *)&mutex);
	i = 0;
	while (i < amount_of_coders)
		pthread_join(thread[i++], NULL);
	// 
	pthread_mutex_destroy(&mutex);
	printf("%d\n", counter);
	if (thread)
		free(thread);
	return 0;
}
