/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quantum_compiler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:00:58 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/11 18:39:11 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

t_scheduler what_is_our_scheldue(char *arg)
{
    if (strcmp(arg, "fifo") == 0)
        return (FIFO);
    else
        return (EDF);
}

// 
void	assign_values(t_quantum_compiler *result, char **argv)
{
	result->coders_c = (int)my_atoi(argv[1]);
    result->burnout_t = (int)my_atoi(argv[2]);
	result->compile_t = (int)my_atoi(argv[3]);
    result->refactor_t = (int)my_atoi(argv[4]);
	result->debug_t = (int)my_atoi(argv[5]);
	result->comp_c_r = (int)my_atoi(argv[6]);
	result->dongle_cd = (int)my_atoi(argv[7]);
	result->scheduler = what_is_our_scheldue(argv[8]);
	result->codersFinished = 0;
	result->burnoutReported = 0;
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
                free(all_dongles[i]->queue);
                free(all_dongles[i]);
                i--;
            }
            free(all_dongles);
			return (NULL);
        }
		i++;
	}
    return (all_dongles);
}

t_coder **init_coders(t_quantum_compiler *state)
{
    int     i;
    t_coder **new_coders;

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
		pthread_mutex_init(&new_coders[i]->time_check, NULL);
        pthread_cond_init(&new_coders[i]->stop_cond, NULL);
        i++;
    }
    return (new_coders);
}

t_quantum_compiler	*init_compiler(int argc, char **argv)
{
	int					i;
	t_quantum_compiler	*result;

    if (!is_scheldue(argv[8]) || !input_check(argc, argv))
        return (NULL);
    result = malloc(sizeof(t_quantum_compiler));
	if (!result)
		return (NULL);
	assign_values(result, argv);
	result->dongles = init_dongles(result);
	if (!result->dongles)
        {free(result); return NULL;}
	result->coders = init_coders(result);
	if (!result->coders)
	{
		i = result->coders_c;
		while (i >= 0)
			free_dongle(result->dongles[i--]);
		free(result);
		return (NULL);
	}
    pthread_cond_init(&result->burnoutSignal, NULL);
    // pthread_cond_init(&result->);
	pthread_mutex_init(&result->burnoutMutex, NULL);
    pthread_mutex_init(&result->state_mutex, NULL);
	pthread_mutex_init(&result->print_m, NULL);
	return (result);
}
