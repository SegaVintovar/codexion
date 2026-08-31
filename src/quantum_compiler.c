# include "codexion.h"

t_scheduler what_is_our_scheldue(char *arg)
{
    if (strcmp(arg, "fifo") == 0)
        return (FIFO);
    else
        return (EDF);
}

int is_scheldue(char *arg)
{
    if (!strcmp(arg, "edf") || !strcmp(arg, "fifo"))
    {
        return (1);
    }
    else
        return (0);
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
        {
            printf("int max test\n"); return (0);
        }
        i++;
	}
	return (1);
}

int	assign_values(t_quantum_compiler *result, int argc, char **argv)
{
	result->coders_c = my_atoi(argv[1]);
    if (result->coders_c < 2)
    {
        printf("not enough coders\n"); return (0);
    }
    result->burnout_t = (int)my_atoi(argv[2]);
	result->compile_t = (int)my_atoi(argv[3]);
    result->debug_t = (int)my_atoi(argv[4]);
	result->refactor_t = (int)my_atoi(argv[5]);
	result->comp_c_r = (int)my_atoi(argv[6]);
	result->dongle_cd = (int)my_atoi(argv[7]);
	return (1);
}

t_quantum_compiler	*init_compiler(int argc, char **argv)
{
	int					i;
	t_quantum_compiler	*result;
	t_scheduler			type;

    if (is_scheldue(argv[8]))
	    type = what_is_our_scheldue(argv[8]);
	else
        return (NULL);
	result = malloc(sizeof(t_quantum_compiler));
	if (!result)
		return (NULL);
    i = 1;
    while (i < argc - 1)
    {
		if (!isint(argv[i]))
        {
			printf("isint\n"); free(result); return NULL;
        };
        i++;
    }
    printf("before innt max check\n");
	if (!int_max_and_positivity_check(argc, argv))
    {
        printf("int max and positivity exit\n"); free(result); return NULL;
    }
	i = assign_values(result, argc, argv);
    printf("after assignation, i = %d\n", i);
    if (i == 0)
    {
        printf("assign values\n"); free(result); return NULL;
    }
	// if (!valid(result))
    //     free(result); return NULL;
	result->scheduler = type;
    // printf("normal return\n");
	return (result);
}

t_dongle    **init_dongles(t_quantum_compiler *instance)
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
        {
            i--;
            while (i >= 0)
            {
                free(all_dongles[i]);
                i--;
            }
            free(all_dongles);
			return (NULL);
        }
        pthread_mutex_init(&all_dongles[i]->mutex, NULL);
		i++;
	}
    return (all_dongles);
}

t_coder **init_coders(t_quantum_compiler *state)
{
    int i;
    t_coder ** new_coders;

    new_coders = malloc(sizeof(t_coder *) * state->coders_c);
    if (!new_coders)
        return NULL;
    i = 0;
    while (i < state->coders_c)
    {
        new_coders[i] = new_coder(i, state);
        if (!new_coders[i])
        {
            i--;
            while (i >= 0)
            {
                free(new_coders[i--]);
            }
            free(new_coders);
            return NULL;
        }
        assign_dongles(new_coders[i], state);
        i++;
    }
    return (new_coders);
}

// not correct
// void    init_threads(t_quantum_compiler *instance)
// {
//     int       i;
//     pthread_t **coders;

//     i = 0;
//     coders = malloc(sizeof(pthread_t) * instance->coders_c);
//     while (i < instance->coders_c)
//     {
//         // idk
//         coders[i] = pthread_init();
//         i++;
//     }
// }

// // not used for now
// void	start(t_quantum_compiler *instance)
// {
// 	// make threads and init dongles
//     // start simulation
// 	init_dongles(instance);
//     init_treads(instance);

// }

