# include "codexion.h"

t_quantum_compiler	*init_compiler(int argc, char **argv)
{
	int					i;
	t_quantum_compiler	*result;
	t_scheduler			type;

	if (strcmp(argv[8], "fifo") == 0)
		type = FIFO;
	else if (strcmp(argv[8], "edf"))
		type = EDF;
	else
		return NULL;
	result = malloc(sizeof(t_quantum_compiler));
	if (!result)
		return NULL;
	// do some checks here or later???
	result->coders_c = atoi(argv[1]);
	result->burnout_t = atoi(argv[2]);
	result->compile_t = atoi(argv[3]);
	result->debug_t = atoi(argv[4]);
	result->refactor_t = atoi(argv[5]);
	result->comp_c_r = atoi(argv[6]);
	result->dongle_cd = atoi(argv[7]);
	result->scheduler = type;

	return (result);
}

void    init_dongles(t_quantum_compiler *instance)
{
    int 		i;
	t_dongle	**all_dongles;

	all_dongles = malloc(sizeof(t_dongle) * instance->coders_c);
    // malloc check
	i = 0;
	while (i < instance->coders_c)
	{
		all_dongles[i] = dongle_new(i);
		i++;
	}
	instance->dongles = all_dongles;
}

void    init_threads(t_quantum_compiler *instance)
{
    int       i;
    pthread_t **coders;

    i = 0;
    coders = malloc(sizeof(pthread_t) * instance->coders_c);
    while (i < instance->coders_c)
    {
        // idk
        coders[i] = pthread_init();
        i++;
    }
}

void	start(t_quantum_compiler *instance)
{
	// make threads and init dongles
    // start simulation
	init_dongles(instance);
    init_treads(instance);

}

