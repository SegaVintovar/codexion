# include "codexion.h"

t_scheduler what_is_our_scheldue(char *arg)
{
	if (strcmp(arg, "fifo") == 0)
		return (FIFO);
	else if (strcmp(arg, "edf") == 0)
		return (EDF);
	else
		return NULL;
}

// check if all the args are under INT_MAX
int	int_max_and_positivity_check(int argc, char **argv)
{
	long	tmp;
	size_t	i;

	i = 1;
	while (i < argc - 2)
	{
		tmp = my_atoi(argv[i]);
		if (tmp > INT_MAX || tmp < 0)
			return (0);
	}
	return (1);
}

int	assign_values(t_quantum_compiler *result, int argc, char **argv)
{
	result->coders_c = my_atoi(argv[1]);
    if (result->coders_c < 2)
        return (0);
	result->burnout_t = my_atoi(argv[2]);
	result->compile_t = my_atoi(argv[3]);
    result->debug_t = my_atoi(argv[4]);
	result->refactor_t = my_atoi(argv[5]);
	result->comp_c_r = my_atoi(argv[6]);
	result->dongle_cd = my_atoi(argv[7]);
	return (1);
}

t_quantum_compiler	*init_compiler(int argc, char **argv)
{
	int					i;
	t_quantum_compiler	*result;
	t_scheduler			type;

	type = what_is_our_scheldue(argv[8]);
	if (!type)
		return NULL;
	result = malloc(sizeof(t_quantum_compiler));
	if (!result)
		return NULL;
    i = 0;
    while (i < argc - 2)
    {
		if (!isint(argv[i]))
			free(result); return NULL;
        i++;
    }
	if (!int_max_and_positivity_check(argc, argv))
		free(result); return NULL;
	if (!assign_values(result, argc, argv))
		free(result); return NULL;
	
	result->scheduler = type;
	return (result);
}

void    init_dongles(t_quantum_compiler *instance)
{
    int 		i;
	t_dongle	**all_dongles;

	all_dongles = malloc(sizeof(t_dongle *) * instance->coders_c);
	if (!all_dongles)
		return (NULL);
	i = 0;
	while (i < instance->coders_c)
	{
		all_dongles[i] = dongle_new(i);
		if (!all_dongles[i])
			return NULL; // and free all
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

