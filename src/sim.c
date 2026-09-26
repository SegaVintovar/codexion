/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:00:53 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/26 11:13:37 by vsudak        ########   odam.nl         */
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

// implement queue where
// coder go
// here I am waiting for the current coder to appear on the first place in the queue
// 
// got a data race

int isNext(t_coder *coder, t_dongle *first, t_dongle *second)
{
    int isNext;

    isNext = 0;
    pthread_mutex_lock(&first->queue_mutex);
    pthread_mutex_lock(&second->queue_mutex);
    if (first->queue[0] == coder && second->queue[0] == coder)
        isNext = 1;
    pthread_mutex_unlock(&first->queue_mutex);
    pthread_mutex_unlock(&second->queue_mutex);
    return (isNext);
}

void    request(t_coder *coder, t_dongle *first, t_dongle *second)
{
    // add this coder to the queue of both dongles
    // pthread_mutex_lock(&second->mutex);
    // pthread_mutex_lock(&first->mutex);
    enque(first, coder);
    enque(second, coder);
    
    // as soon as this coder is in queue[0] of the both dongles
    while (isNext(coder, first, second) == 0)
        usleep(500);
    pop(first);
    pop(second);
    // pthread_mutex_unlock(&second->mutex);
    // pthread_mutex_unlock(&first->mutex);
}

int	dongleAcquisition(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	oneTwoMutex(coder, &first, &second);
    request(coder, first, second);
	// should I stay or should I go?
	// waiting mechanics to start geting dongles
	if (grabDOngle(first, coder) == 1)
        return (1);
	if (second != first)
	{
        if (grabDOngle(second, coder) == 1)
		{
			pthread_mutex_unlock(&first->mutex);
			return (1);
		}
    }
	else
		usleep(converter(coder->state->burnout_t));
	if (isBurned(coder->state, coder) == 1)
		return (dropDongles(coder), 1);
	return (0);
}

void set_last_comp_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->state->last_comp_t_mutex);
	coder->last_comp_t = curtime_full();
    pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
}

void *sim(void *coder)
{
	int					i;
    t_coder				*c;
	
	c =(t_coder *)coder;
	set_last_comp_time(c);
	i = 0;
	while (i < c->state->comp_c_r && !isBurned(c->state, coder))
	{
		if (dongleAcquisition(c) == 1)
			return (burnoutReport(c->state, coder, 0), NULL);
		if (new_comp(coder) == 1)
			return (dropDongles(coder), burnoutReport(c->state, coder, 0), \
			NULL);
		else
			dropDongles(coder);
		if (debugging(coder, c->state) == 1)
			return (burnoutReport(c->state, coder, 0), NULL);
		if (refactoring(coder, c->state) == 1)
			return (burnoutReport(c->state, coder, 0), NULL);
		i++;
	}
    coderFinished(c->state);
	return (NULL);
}


// I need to check pthread creat return value and I need to handle it
void	start_batch_of_coders(t_quantum_compiler *state, int batch)
{
	int             i;
    t_coder         *c;
	
	i = 0;
    while (i < state->coders_c)
    {
		if (i % 2 == batch)
		{
			c = state->coders[i];
			pthread_create(&state->coders[i]->thread, NULL, sim, (void *)c);
		}
        i++;
    }
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

void run(t_quantum_compiler *state)
{
	int		i;
	t_coder	*c;

	start_monitor(state);
	set_the_time(state);
    start_batch_of_coders(state, 0);
	start_batch_of_coders(state, 1);

    i = 0;
    while (i < state->coders_c)
    {
        c = state->coders[i];
        pthread_join(c->thread, NULL);
        i++;
    }
    stop_monitor(state);
}
