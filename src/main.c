/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 11:55:27 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/08 19:39:03 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void    free_dongles(t_quantum_compiler *state)
{
    int i;

    if (state->dongles)
        {
            i = 0;
            while (i < state->coders_c)
            {
                if (state->dongles[i])
                {
                    pthread_mutex_destroy(&state->dongles[i]->mutex);
                    free(state->dongles[i]);
                }
                i++;
            }
            free(state->dongles);
        }
}

void    free_coders(t_quantum_compiler *state)
{
    int i;

    if (state->coders)
        {
            i = 0;
            while (i < state->coders_c)
            {
                if (state->coders[i])
                    free(state->coders[i]);
                i++;
            }
            free(state->coders);
        }
}

void    free_all(t_quantum_compiler *state)
{
    if (state)
    {
        free_dongles(state);
        free_coders(state);
        pthread_cond_destroy(&state->burnoutSignal);
		pthread_mutex_destroy(&state->burnoutMutex);
		pthread_mutex_destroy(&state->print_m);
        free(state);
    }
}

// to allocate the queue
// void    queueStart(t_quantum_compiler *state)
// {
//     int i;

    
    
//     if (state->scheduler == EDF)
//     {
//         // here we need a checker that will tell us which coder is closer to burnout   
//     }
//     else
//     {
//         // here we can have a queue
//     }
//     i = 0;
    
// }

int	main(int argc, char **argv)
{
    t_quantum_compiler  *state;

    if (argc != 9)
    {
        printf("The program is expecting 8 arguments\n");
        return (1);
    }
    state = init_compiler(argc, argv);
    if (!state)
        return (1);
    // queueStart(state);
    run(state);
	// if (state->burnoutReported)
	// {
	// 	printf("%lu %d got burnout\n", state->whenWeGotBurn, state->whoGotBurned);
	// }
    free_all(state);
}
