/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:00:53 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/11 18:41:51 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// here we are getting rid of deadlock
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
// void    compiling(t_coder *coder, t_quantum_compiler *state)
// {
// 	t_dongle	*first;
// 	t_dongle	*second;

//     // first = NULL;
//     // second = NULL;
// 	oneTwoMutex(coder, &first, &second);
// 	if (first == second)
//     {
//         usleep(converter((uint64_t)state->burnout_t));
//         return (burnoutReport(state, coder, 0));    
//     }

// 	if (burnoutCheck(state, coder))
// 		return;
// 	dongle_lock(first, coder);
// 	if (burnoutReportCheck(state))
// 		return ((void)pthread_mutex_unlock(&first->mutex));
// 	else
// 		safePrint(state, coder, "taken a dongle");
// 	dongle_lock(second, coder);
// 	if (burnoutReportCheck(state))
// 	{
// 		pthread_mutex_unlock(&first->mutex);
// 		pthread_mutex_unlock(&second->mutex);
// 		return;
// 	}
// 	else
// 		safePrint(state, coder, "taken a dongle");
//     coder->last_comp_t = curtime_full();
// 	safePrint(state, coder, "started compiling");
//     usleep(converter((uint64_t)state->compile_t));
//     coder->compiles_left--;
//     coder->left->avaliable_at = curtime_full() + (uint64_t)state->dongle_cd;
//     dongle_unlock(coder->left);
//     coder->right->avaliable_at = curtime_full() + (uint64_t)state->dongle_cd;
//     dongle_unlock(coder->right);
// }

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

// void *simulation(t_coder *coder, t_quantum_compiler *state)
// void *simulation(void *coder)
// {
//     int                 i;
//     t_coder             *c;

//     c =(t_coder *)coder;
//     i = 0;
//     while (i < c->state->comp_c_r && !burnoutReportCheck(c->state)) // and there is no burnout signal
//     {
//         compiling(c, c->state);
//         if (burnoutReportCheck(c->state))
//             break;
//         refactoring(c, c->state);
//         if (burnoutReportCheck(c->state))
//             break;
//          debugging(c, c->state);
//         i++;
//     }
// 	if (c->state->comp_c_r == i)
// 	{
// 		pthread_mutex_lock(&c->state->burnoutMutex);
// 		c->state->codersFinished += 1;
// 		pthread_cond_signal(&c->state->burnoutSignal);
// 		pthread_mutex_unlock(&c->state->burnoutMutex);
// 	}
//     return (NULL);
// }

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

void dropDongles(t_coder *coder)
{
	uint64_t	now;
	uint64_t	avail_at;

	now = curtime_full();
	avail_at = now + coder->state->dongle_cd;
	coder->left->avaliable_at = avail_at;
	coder->right->avaliable_at = avail_at;
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);

}

int	dongleAcquisition(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	uint64_t	now;

	oneTwoMutex(coder, &first, &second);
	// checking when the dongle is avaliable
	// we can expect burnout while we are waiting for the dongle to become avaliable
	// it could be locked or (avaliable_at > now)
	// avaliable at
	now = curtime_full();
	pthread_mutex_lock(&first->mutex);
	if (first->avaliable_at > now) // data race ;-(
		usleep(first->avaliable_at - now);
	if (isBurned(coder->state, coder) == 1)
		return (1);
	
	safePrint(coder->state, coder, "taken first dongle");
	if (isBurned(coder->state, coder) == 1)
		return (pthread_mutex_unlock(&first->mutex), 1);
	// we got it, but how long did it take to get a dongle
	// so we are checking burnout again
	pthread_mutex_lock(&second->mutex);
	safePrint(coder->state, coder, "taken second dongle");
	// check again, because dongle could be buzy or on cd
	// if (isBurned(coder->state, coder) == 1)
	// 	return (dropDongles(coder), 1);
	return (0);
}

void coderFinished(t_quantum_compiler *state)
{
    pthread_mutex_lock(&state->state_mutex);
    state->codersFinished++;
    pthread_mutex_unlock(&state->state_mutex);
}

void *sim(void *coder)
{
	int					i;
    t_coder				*c;
	t_quantum_compiler	*state;
	
	c =(t_coder *)coder;
	c->last_comp_t = curtime_full();
    state = c->state;
	i = 0;
	while (i < state->comp_c_r && !isBurned(state, coder))
	{
		if (dongleAcquisition(c) == 1)
			return (burnoutReport(c->state, coder, 0), NULL);
		if (new_comp(coder) == 1)
			return (dropDongles(coder), burnoutReport(c->state, coder, 0), NULL);
		else
			dropDongles(coder);
		if (debugging(coder, c->state) == 1)
			return (burnoutReport(c->state, coder, 0), NULL);
		if (refactoring(coder, c->state) == 1)
			return (burnoutReport(c->state, coder, 0), NULL);
		i++;
	}
    coderFinished(state);
    if (i == state->comp_c_r)
        {pthread_cond_signal(&c->stop_cond);}
	return (NULL);
}

// here we are setting the start time for the state and for all coders
void	set_the_time(t_quantum_compiler *state)
{
	uint64_t	now;
	int			i;
	now = curtime_full();
	state->start_time = now;
	i = 0;
	while (i < state->coders_c)
	{
		state->coders[i]->last_comp_t = now;
		i++;
	}
}

void run(t_quantum_compiler *state)
{
    int             i;
    t_coder         *c;

    
	start_monitor(state);
	set_the_time(state);
    i = 0;
    while (i < state->coders_c)
    {
		if (i % 2 == 0)
		{
			c = state->coders[i];
			pthread_create(&state->coders[i]->thread, NULL, sim, (void *)c);
		}
        i++;
    }
    i = 0;
    while (i < state->coders_c)
    {
		if (i % 2 != 0)
		{
			c = state->coders[i];
			pthread_create(&state->coders[i]->thread, NULL, sim, (void *)c);
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
