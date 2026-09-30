/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   deadline.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/30 12:13:07 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/30 12:13:11 by vsudak        ########   odam.nl         */
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

// waits until the dongle is free and claims it (only plain lock/unlock +
// usleep). The mutex just guards the flags, it is never held while waiting.
// returns 1 if the coder burned out while waiting
int	claim_dongle(t_dongle *dongle, t_coder *coder, uint64_t *avail_at)
{
	while (1)
	{
		pthread_mutex_lock(&dongle->mutex);
		if (dongle->buzy == 0)
		{
			dongle->buzy = 1;
			*avail_at = dongle->avaliable_at;
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		pthread_mutex_unlock(&dongle->mutex);
		if (is_burned(coder->state, coder) == 1)
			return (1);
		usleep(500);
	}
}

// frees the dongle, avail_at == 0 keeps the old cooldown
void	release_dongle(t_dongle *dongle, uint64_t avail_at)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->buzy = 0;
	if (avail_at != 0)
		dongle->avaliable_at = avail_at;
	pthread_mutex_unlock(&dongle->mutex);
}
