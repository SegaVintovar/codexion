/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/07/24 11:55:27 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/05 12:06:44 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// void *coder(t_quantum_compiler *state)
// {

//     return NULL;
// }

// // I need a struct to pass it to the working func

// void *worker(void *arg)
// {
//     // printf("Doing a job\n");
//     struct timeval      curtime;
//     uint64_t            curtime_full;
//     t_dongle            *dongle;
//     pthread_mutex_t     *mutex;

//     gettimeofday(&curtime, NULL);
//     curtime_full = (curtime.tv_sec * (uint64_t)1000) + (curtime.tv_usec / 1000);
//     // i need to change argument to quantum_compiler
//     // so i can get more data
//     mutex = (pthread_mutex_t *)arg;
//     // mutex = dongle->mutex;
//     // usleep(1000000);
//     pthread_mutex_lock(mutex);
        
//     printf("Since Unix Epoch(miliseconds): %ld\n", ((long)curtime_full));
//     long years = (long)curtime_full / 31556952000;
//     printf("Since Unix Epoh(years): %ld\n", years);
//     // dongle->time = curtime_full;
//     pthread_mutex_unlock(mutex);
//     return NULL;
// }

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
    // state->dongles = init_dongles(state);
    // if (!state->dongles)
    //     {
    //         free(state);
    //         return(1);
    //     }
    // state->coders = init_coders(state);
    // if (!state->coders)
    // {
    //     free_all(state);
    //     return(1);
    // }
    run(state);
    free_all(state);
}
