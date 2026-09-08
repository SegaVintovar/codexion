/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:00:44 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/08 11:54:38 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

t_dongle	*dongle_new(int id)
{
    t_dongle    *new;

    new = malloc(sizeof(t_dongle));
    if  (!new)
        return NULL;
    // new->locked = false;
    new->id = id;
    pthread_mutex_init(&new->mutex, NULL);
    return new;
}

void dongle_lock(t_dongle *dongle)
{
    uint64_t    now;

    if (dongle)
    {
		pthread_mutex_lock(&dongle->mutex);
        now = curtime_full();
        if (dongle->avaliable_at > now)
            usleep(dongle->avaliable_at - now);
    }
}

void dongle_unlock(t_dongle * dongle)
{
    if (dongle)
	{
        
		pthread_mutex_unlock(&dongle->mutex);
	}
}

// this one will go into free all
void free_dongle(t_dongle *dongle)
{
    if (dongle)
    {
        pthread_mutex_destroy(&dongle->mutex);
        free(dongle);
    }
}
