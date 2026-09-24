/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 15:04:25 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/17 22:10:15 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// to convert from miliseconds into microseconds
uint64_t    converter(uint64_t t)
{
	return (t * 1000);
}

// return full current time in miliseconds
uint64_t    curtime_full()
{
    struct timeval		curtime;
	uint64_t			curtime_full;

	gettimeofday(&curtime, NULL);
	curtime_full = (curtime.tv_sec * (uint64_t)1000) + (curtime.tv_usec / 1000);
    return (curtime_full);
}

uint64_t    time_scince_start(t_quantum_compiler *state)
{
    uint64_t    result;

    result = curtime_full() - state->start_time;
    return (result);
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

// here i am sleeping till end of the dongle cd or coder`s bo
uint64_t	sleep_cd(t_dongle *dongle, t_coder *coder, uint64_t now)
{
	uint64_t	to_sleep;
	uint64_t	bot;

    pthread_mutex_lock(&coder->state->last_comp_t_mutex);
    bot = coder->last_comp_t + (uint64_t)coder->state->burnout_t;
    pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
    if (bot > now)
    {
        if (willBeBurned(coder, dongle))
            to_sleep = bot - now;
        else
            to_sleep = dongle->avaliable_at - now;
        return (to_sleep);
    }
    return (0);
}
