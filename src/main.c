/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/07/24 11:55:27 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/05 13:02:44 by vsudak        ########   odam.nl         */
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
        free(state);
    }
}

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
    run(state);
    free_all(state);
}
