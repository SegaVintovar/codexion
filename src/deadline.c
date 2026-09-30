/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deadline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 20:22:40 by vs                #+#    #+#             */
/*   Updated: 2026/09/30 10:17:35 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// absolute burnout deadline of this coder (ms)
uint64_t	deadline_of(t_coder *coder)
{
	uint64_t	dl;

	pthread_mutex_lock(&coder->state->last_comp_t_mutex);
	dl = coder->last_comp_t + (uint64_t)coder->state->burnout_t;
	pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
	return (dl);
}

// how long (ms) the coder may wait for `want` ms without passing its own
// deadline: never longer than the time it has left
uint64_t	cap_to_deadline(t_coder *coder, uint64_t want)
{
	uint64_t	now;
	uint64_t	dl;

	now = curtime_full();
	dl = deadline_of(coder);
	if (dl <= now)
		return (0);
	if (now + want > dl)
		return (dl - now);
	return (want);
}

// mutex lock that gives up at the coder's deadline, returns 1 on timeout
int	lock_before_deadline(t_coder *coder, pthread_mutex_t *mutex)
{
	struct timespec	ts;
	uint64_t		dl;

	dl = deadline_of(coder);
	ts.tv_sec = dl / 1000;
	ts.tv_nsec = (dl % 1000) * 1000000;
	if (pthread_mutex_timedlock(mutex, &ts) == 0)
		return (0);
	return (1);
}
