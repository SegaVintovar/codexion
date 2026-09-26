/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:00:40 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/26 15:25:48 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *arg)
{
	t_quantum_compiler *state;
	int	report;

	report = 0;
	state = (t_quantum_compiler *)arg;
	pthread_mutex_lock(&state->burnoutMutex);
	
	while (!state->burnoutReported && state->coders_c != state->codersFinished)
	{
		pthread_cond_wait(&state->burnoutSignal, &state->burnoutMutex);
	}
	state->burnoutReported = 1;
	report = state->burnoutReported;
	
	if (report && state->coders_c != state->codersFinished)
	{
		pthread_mutex_lock(&state->print_m);
		printf("%lu %d burned out\n", state->whenWeGotBurn, (state->whoGotBurned + 1));
		pthread_mutex_unlock(&state->print_m);
	}
	pthread_mutex_unlock(&state->burnoutMutex);
	return NULL;
}

int	start_monitor(t_quantum_compiler *state)
{
	if (pthread_create(&state->monitor_thread, NULL, monitor, (void *)state))
		return (1);
	return (0);
}

void    stop_monitor(t_quantum_compiler *state)
{
    pthread_join(state->monitor_thread, NULL);
}
