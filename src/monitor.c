/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   monitor.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: vs <vs@student.42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/09/05 13:00:40 by vsudak			#+#	#+#			 */
/*   Updated: 2026/09/29 14:01:51 by vs			   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *arg)
{
	t_quantum_compiler	*state;
	int					report;

	report = 0;
	state = (t_quantum_compiler *)arg;
	pthread_mutex_lock(&state->burnout_mutex);
	while (
		!state->burnout_reported && state->coders_c != state->coders_finished)
	{
		pthread_cond_wait(&state->burnout_sig, &state->burnout_mutex);
	}
	state->burnout_reported = 1;
	report = state->burnout_reported;
	if (report && state->coders_c != state->coders_finished)
	{
		pthread_mutex_lock(&state->print_m);
		printf("%lu %d burned out\n", state->when_we_got_burned,
			(state->who_got_burned + 1));
		pthread_mutex_unlock(&state->print_m);
	}
	pthread_mutex_unlock(&state->burnout_mutex);
	return (NULL);
}

int	start_monitor(t_quantum_compiler *state)
{
	if (pthread_create(&state->monitor_thread, NULL, monitor, (void *)state))
		return (1);
	return (0);
}

void	stop_monitor(t_quantum_compiler *state)
{
	pthread_join(state->monitor_thread, NULL);
}
