/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/07/24 11:55:27 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/03 14:31:02 by vsudak        ########   odam.nl         */
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
    state->dongles = init_dongles(state);
    if (!state->dongles)
        {
            free(state);
            return(1);
        }
    state->coders = init_coders(state);
    if (!state->coders)
    {
        free_all(state);
        return(1);
    }
    run(state);
    free_all(state);
}

// old shit
// void *work(void *arg)
// {
//     // we are working while all coders did not reach required amount of compilations
//     // or burnout...
//     // learn how to send signals and use conditions
//     t_quantum_compiler  *state;

//     state = (t_quantum_compiler *)arg;
    
// }




// void old_stuff()
// {    
//  pthread_mutex_init(&mutex, NULL);
//  amount_of_coders = atoi(argv[1]);
//     amount_of_compilations = atoi(argv[2]);
//     state->coders_c = amount_of_coders;
//     state->comp_c_r = amount_of_compilations;
//  coders = malloc(sizeof(t_coder) * amount_of_coders);
//     // give dongles to the coders
//     state->coders = coders;
//     // i = 0;
//     // while (i < amount_of_coders)
//     // {
//     //     coders[i]->coder = malloc(sizeof(pthread_t));
//     //     i++; 
//     // }
//  // I think this part goes under another loop that counts amount of the compilations
//     while (amount_of_compilations)
//     {
//         i = 0;
//         while (i < amount_of_coders)
//      {
//             pthread_create(&coders[i++].coder, NULL, worker, (void *)&mutex);
//      }
//      i = 0;
//         while (i < amount_of_coders)
//             pthread_join(coders[i++].coder, NULL);
//         amount_of_compilations--;
//     }
//  pthread_mutex_destroy(&mutex);
//  if (coders)
//      free(coders);
//  return 0;
// }
