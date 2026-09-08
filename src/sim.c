/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:00:53 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/08 11:04:07 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// here we are reporting about burnout(stop)
void    burnoutReport(t_quantum_compiler *state, t_coder *coder)
{
    pthread_mutex_lock(&state->burnoutMutex);
    state->burnoutReported = 1;
    state->whoGotBurned = coder->id;
    state->whenWeGotBurn = curtime_full() - state->start_time;
    pthread_cond_signal(&state->burnoutSignal); // we got burnout
    pthread_mutex_unlock(&state->burnoutMutex);
}

// here we are checking if burnout was already reported
int burnoutReportCheck(t_quantum_compiler *state)
{
    int result;
    
    pthread_mutex_lock(&state->burnoutMutex);
    result = state->burnoutReported;
    pthread_mutex_unlock(&state->burnoutMutex);
    return (result);
}

// For compiling top check if current coder is not burnedout
int	burnoutCheck(t_quantum_compiler *state, t_coder *coder)
{
	if (!coder->compiles_left || burnoutReportCheck(state))
        return (1);
    if (coder->compiles_left == state->comp_c_r) // first start
    {
        if ((curtime_full() - state->start_time) > (uint64_t)state->burnout_t) // ->
            {
				burnoutReport(state, coder);
				return (1);
			}
	}
    else if ((curtime_full() - coder->last_comp_t) > \
        (uint64_t)state->burnout_t && \
        coder->compiles_left != state->comp_c_r)
    {
        burnoutReport(state, coder);
		return (1);
    }
	return (0);
}

// here we are getting reed of deadlock
// basically first coder takes dongles in different order
void	oneTwoMutex(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left->id < coder->right->id)
	{
	    *first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

// First stop of our routine
// the most difficult one, cause of access to the shared dongles
void    compiling(t_coder *coder, t_quantum_compiler *state)
{
	t_dongle	*first;
	t_dongle	*second;

    // first = NULL;
    // second = NULL;
	oneTwoMutex(coder, &first, &second);
	if (first == second)
    {
        usleep(converter((uint64_t)state->burnout_t));
        return (burnoutReport(state, coder));    
    }
	
	if (burnoutCheck(state, coder))
		return;
	dongle_lock(first);
	if (burnoutReportCheck(state))
		return ((void)pthread_mutex_unlock(&first->mutex));
	else
		safePrint(state, coder, "taken a dongle");
	dongle_lock(second);
	if (burnoutReportCheck(state))
	{
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return;
	}
	else
		safePrint(state, coder, "taken a dongle");
    coder->last_comp_t = curtime_full();
	safePrint(state, coder, "started compiling");
    usleep(converter((uint64_t)state->compile_t));
    coder->compiles_left--;
    coder->left->avaliable_at = curtime_full() + (uint64_t)state->dongle_cd;
    dongle_unlock(coder->left);
    coder->right->avaliable_at = curtime_full() + (uint64_t)state->dongle_cd;
    dongle_unlock(coder->right);
}

void    refactoring(t_coder *coder, t_quantum_compiler *state)
{
	safePrint(state, coder, "started refactoring");
    usleep(converter((uint64_t)state->refactor_t));
}

void    debugging(t_coder *coder, t_quantum_compiler *state)
{
	safePrint(state, coder, "started debugging");
    usleep(converter((uint64_t)state->debug_t));
}

// void *simulation(t_coder *coder, t_quantum_compiler *state)
void *simulation(void *coder)
{
    int                 i;
    t_coder             *c;

    c =(t_coder *)coder;
    i = 0;
    while (i < c->state->comp_c_r && !burnoutReportCheck(c->state)) // and there is no burnout signal
    {
        compiling(c, c->state);
        if (burnoutReportCheck(c->state))
            break;
        refactoring(c, c->state);
        if (burnoutReportCheck(c->state))
            break;
         debugging(c, c->state);
        i++;
    }
	if (c->state->comp_c_r == i)
	{
		pthread_mutex_lock(&c->state->burnoutMutex);
		c->state->codersFinished += 1;
		pthread_cond_signal(&c->state->burnoutSignal);
		pthread_mutex_unlock(&c->state->burnoutMutex);
	}
    return (NULL);
}

void run(t_quantum_compiler *state)
{
    int             i;
    t_coder         *c;

    state->start_time = curtime_full();
	start_monitor(state);
    i = 0;
    while (i < state->coders_c)
    {
		if (i % 2 == 0)
		{
			c = state->coders[i];
			pthread_create(&state->coders[i]->thread, NULL, simulation, (void *)c);
		}
        i++;
    }
    i = 0;
    while (i < state->coders_c)
    {
		if (i % 2 != 0)
		{
			c = state->coders[i];
			pthread_create(&state->coders[i]->thread, NULL, simulation, (void *)c);
		}
		i++;
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
