/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   sim.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: vs <vs@student.42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/09/05 13:00:53 by vsudak			#+#	#+#			 */
/*   Updated: 2026/09/29 12:33:04 by vs			   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "codexion.h"

void	set_last_comp_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->state->last_comp_t_mutex);
	coder->last_comp_t = curtime_full();
	pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
}

t_coder	*take_on_board(void *coder)
{
	t_coder	*c;

	c = (t_coder *)coder;
	set_last_comp_time(c);
	return (c);
}

void	*sim(void *coder)
{
	int		i;
	t_coder	*c;

	c = take_on_board(coder);
	i = 0;
	while (i < c->state->comp_c_r && !is_burned(c->state, coder))
	{
		if (dongle_acquisition(c) == 1)
			return (burnout_report(c->state, coder, 0), NULL);
		if (new_comp(coder) == 1)
			return (drop_dongles(coder), burnout_report(c->state, coder, 0),
				NULL);
		else
			drop_dongles(coder);
		if (debugging(coder, c->state) == 1)
			return (burnout_report(c->state, coder, 0), NULL);
		if (refactoring(coder, c->state) == 1)
			return (burnout_report(c->state, coder, 0), NULL);
		i++;
	}
	if (c->state->comp_c_r == i)
		coder_finished(c->state);
	else
		burnout_report(c->state, coder, 0);
	return (NULL);
}

// void	start_all_threads(t_quantum_compiler *state)
// {
// 	int	i;
// 	i = 0;
// 	while (i < state->coders)
// 	{
// 		pthread_create(&state->coders[i]->thread, NULL, sim, (void *)c);
// 		i++;
// 	}
// }

void	run(t_quantum_compiler *state)
{
	int		i;
	t_coder	*c;

	if (start_monitor(state))
		return ;
	set_the_time(state);
	if (start_batch_of_coders(state, 0) != 0)
		return ;
	if (start_batch_of_coders(state, 1) != 0)
	{
		join_first_batch(state);
		return ;
	}
	i = 0;
	while (i < state->coders_c)
	{
		c = state->coders[i];
		pthread_join(c->thread, NULL);
		i++;
	}
	stop_monitor(state);
}
