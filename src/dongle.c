/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:00:44 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/16 21:36:05 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

t_dongle	*dongle_new(int id)
{
    t_dongle    *new;

    new = malloc(sizeof(t_dongle));
    if  (!new)
        return NULL;
    new->id = id;
    new->avaliable_at = 0;
    new->buzy = 0;
    pthread_mutex_init(&new->mutex, NULL);
    new->queue = initQueue();
    if (!new->queue)
    {
        pthread_mutex_destroy(&new->mutex);
        free(new);
        return (NULL);
    }
    return new;
}


void	dongle_unlock(t_dongle * dongle)
{
    if (dongle)
	{
		pthread_mutex_unlock(&dongle->mutex);
	}
}

// this one will go into free all
void	free_dongle(t_dongle *dongle)
{
    if (dongle)
    {
        pthread_mutex_destroy(&dongle->mutex);
        free(dongle);
    }
}

void dropDongles(t_coder *coder)
{
	uint64_t	now;
	uint64_t	avail_at;

	now = curtime_full();
	avail_at = now + coder->state->dongle_cd;
	coder->left->avaliable_at = avail_at;
	coder->right->avaliable_at = avail_at;
	coder->left->avaliable_at = avail_at;
	coder->right->buzy = 0;
	if (coder->left == coder->right)
	{
		pthread_mutex_unlock(&coder->left->mutex);
	}
	else
	{
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
	}
}

int grabDOngle(t_dongle *first, t_coder *coder)
{
    uint64_t	now;
    
    pthread_mutex_lock(&first->mutex);
	now = curtime_full();
	if (first->avaliable_at > now)
		usleep(converter(sleep_cd(first, coder, now)));
	if (isBurned(coder->state, coder) == 1)
		return (pthread_mutex_unlock(&first->mutex), 1);
	safePrint(coder->state, coder, "taken dongle");
    first->buzy = 1;
	if (isBurned(coder->state, coder) == 1)
		return (pthread_mutex_unlock(&first->mutex), 1);
    return (0);
}