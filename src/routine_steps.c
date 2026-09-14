/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine_steps.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/14 18:31:09 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/14 18:40:06 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	new_comp(t_coder *coder)
{
	uint64_t	comp_t;
	uint64_t	now;
	
	pthread_mutex_lock(&coder->time_check);
	now = curtime_full();
	comp_t = (uint64_t)coder->state->compile_t;
	coder->last_comp_t = now;
	pthread_mutex_unlock(&coder->time_check);
	safePrint(coder->state, coder, "started compiling");
	usleep(converter(comp_t));
	return (isBurned(coder->state, coder));
}

int    refactoring(t_coder *coder, t_quantum_compiler *state)
{
	safePrint(state, coder, "started refactoring");
    usleep(converter((uint64_t)state->refactor_t));
	if (isBurned(state, coder))
		return (1);
	return (0);
}

int    debugging(t_coder *coder, t_quantum_compiler *state)
{
	safePrint(state, coder, "started debugging");
    usleep(converter((uint64_t)state->debug_t));
	if (isBurned(state, coder))
		return (1);
	return (0);
}


void coderFinished(t_quantum_compiler *state)
{
    pthread_mutex_lock(&state->burnoutMutex);
    state->codersFinished++;
	if (state->coders_c == state->codersFinished)
		pthread_cond_signal(&state->burnoutSignal);
    pthread_mutex_unlock(&state->burnoutMutex);
}


// will it be burned at the during dongle_cd
int	willBeBurned(t_coder *coder, t_dongle *dongle)
{
	if (coder->last_comp_t + coder->state->burnout_t < dongle->avaliable_at)
		return (1);
	else
		return (0);	
}