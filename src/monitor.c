/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:00:40 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/10 18:44:36 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *arg)
{
	t_quantum_compiler *state;

	state = (t_quantum_compiler *)arg;
	pthread_mutex_lock(&state->burnoutMutex);
	
	while (!state->burnoutReported && state->coders_c > state->codersFinished)
	{
		pthread_cond_wait(&state->burnoutSignal, &state->burnoutMutex);
	}
	pthread_cond_broadcast(&state->burnoutSignal);
	state->burnoutReported = 1;
	
	if (state->burnoutReported && state->coders_c != state->codersFinished)
	{
		pthread_mutex_lock(&state->print_m);
		printf("%lu %d got burnout\n", state->whenWeGotBurn, state->whoGotBurned);
		pthread_mutex_unlock(&state->print_m);
	}
	pthread_mutex_unlock(&state->burnoutMutex);
	return NULL;
}

void	start_monitor(t_quantum_compiler *state)
{
	pthread_create(&state->monitor_thread, NULL, monitor, (void *)state);	
}

void    stop_monitor(t_quantum_compiler *state)
{
    pthread_join(state->monitor_thread, NULL);
}
