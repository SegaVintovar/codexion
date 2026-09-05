/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:00:40 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/05 17:08:53 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *arg)
{
	t_quantum_compiler *state;

	state = (t_quantum_compiler *)arg;
	pthread_mutex_lock(&state->burnoutMutex);
	while (!state->burnoutReported)
	{
		pthread_cond_wait(&state->burnoutSignal, &state->burnoutMutex);
	}
	pthread_cond_broadcast(&state->burnoutSignal);
	state->should_stop = 1;
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
