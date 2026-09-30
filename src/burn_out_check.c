/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   burnOutCheck.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/29 12:24:07 by vs            #+#    #+#                 */
/*   Updated: 2026/09/30 12:11:38 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_is_finished(t_coder *coder)
{
	if (coder->state->coders_c == coder->state->coders_finished)
		return (1);
	else
		return (0);
}

// check if coder is burned
// I dont report here!!!
// or not finished
int	is_burned(t_quantum_compiler *state, t_coder *coder)
{
	uint64_t	now;
	uint64_t	last_comp;

	pthread_mutex_lock(&coder->state->burnout_mutex);
	if (coder->state->coders_c == coder->state->coders_finished)
	{
		pthread_cond_signal(&state->burnout_sig);
		pthread_mutex_unlock(&coder->state->burnout_mutex);
		return (1);
	}
	now = curtime_full();
	pthread_mutex_lock(&coder->state->last_comp_t_mutex);
	last_comp = coder->last_comp_t;
	pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
	pthread_mutex_unlock(&coder->state->burnout_mutex);
	if (now - last_comp >= (uint64_t)state->burnout_t)
		return (1);
	else
	{
		pthread_mutex_lock(&state->burnout_mutex);
		if (state->burnout_reported == 1)
			return (pthread_mutex_unlock(&state->burnout_mutex), 1);
		return (pthread_mutex_unlock(&state->burnout_mutex), 0);
	}
}

// here we are reporting about burnout(stop)
void	burnout_report(t_quantum_compiler *state, t_coder *coder, uint64_t when)
{
	pthread_mutex_lock(&state->burnout_mutex);
	if (state->burnout_reported)
	{
		pthread_mutex_unlock(&state->burnout_mutex);
		return ;
	}
	state->burnout_reported = 1;
	state->who_got_burned = coder->id;
	if (when == 0)
		state->when_we_got_burned = curtime_full() - state->start_time;
	else
		state->when_we_got_burned = when;
	pthread_cond_signal(&state->burnout_sig);
	pthread_mutex_unlock(&state->burnout_mutex);
}

// here we are checking if burnout was already reported
int	burnout_rep_check(t_quantum_compiler *state)
{
	int	result;

	pthread_mutex_lock(&state->burnout_mutex);
	result = state->burnout_reported;
	pthread_mutex_unlock(&state->burnout_mutex);
	return (result);
}

// will it be burned during next activity
// int	will_b_burned(t_coder *coder, uint64_t next_activity)
// {
// 	int			result;
// 	uint64_t	now;

// 	pthread_mutex_lock(&coder->state->last_comp_t_mutex);
// 	now = curtime_full();
// 	if (coder->last_comp_t + coder->state->burnout_t < now + next_activity)
// 		result = 1;
// 	else
// 		result = 0;
// 	pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
// 	return (result);
// }
// For compiling top check if current coder is not burnedout
// not used anymore
// int	burnoutCheck(t_quantum_compiler *state, t_coder *coder)
// {
// 	if (!coder->compiles_left || burnout_rep_check(state))
//		 return (1);
//	 if (coder->compiles_left == state->comp_c_r) // first start
//	 {
//		 if ((curtime_full() - state->start_time) > (uint64_t)state->burnout_t)
//		 {
//			 burnout_report(state, coder, 0);
//			 return (1);
//		 }
// 	}
//	 else if ((curtime_full() - coder->last_comp_t) > 
//		 (uint64_t)state->burnout_t && 
//		 coder->compiles_left != state->comp_c_r)
//	 {
//		 burnout_report(state, coder, 0);
// 		return (1);
//	 }
// 	return (0);
// }
