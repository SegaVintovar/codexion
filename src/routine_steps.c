/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   routine_steps.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: vs <vs@student.42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/09/14 18:31:09 by vsudak			#+#	#+#			 */
/*   Updated: 2026/09/29 12:30:43 by vs			   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "codexion.h"

int	new_comp(t_coder *coder)
{
	uint64_t	comp_t;

	pthread_mutex_lock(&coder->time_check);
	comp_t = (uint64_t)coder->state->compile_t;
	pthread_mutex_unlock(&coder->time_check);
	set_last_comp_time(coder);
	safe_print(coder->state, coder, "started compiling");
	usleep(converter(cap_to_deadline(coder, comp_t)));
	if (is_burned(coder->state, coder))
		return (1);
	return (0);
}

int	refactoring(t_coder *coder, t_quantum_compiler *state)
{
	uint64_t	t2sleep;

	t2sleep = cap_to_deadline(coder, (uint64_t)state->refactor_t);
	safe_print(state, coder, "started refactoring");
	usleep(converter(t2sleep));
	if (is_burned(state, coder))
		return (1);
	return (0);
}

int	debugging(t_coder *coder, t_quantum_compiler *state)
{
	uint64_t	t2sleep;

	t2sleep = converter(cap_to_deadline(coder, (uint64_t)state->debug_t));
	safe_print(state, coder, "started debugging");
	usleep(t2sleep);
	if (is_burned(state, coder))
		return (1);
	return (0);
}

void	coder_finished(t_quantum_compiler *state)
{
	pthread_mutex_lock(&state->burnout_mutex);
	state->coders_finished++;
	if (state->coders_c == state->coders_finished)
		pthread_cond_signal(&state->burnout_sig);
	pthread_mutex_unlock(&state->burnout_mutex);
}
