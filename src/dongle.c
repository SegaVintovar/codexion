/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dongle.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:00:44 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/07 16:42:24 by vsudak        ########   odam.nl         */
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

void dongle_lock(pthread_mutex_t *dongle)
{
    if (dongle)
		pthread_mutex_lock(dongle);
}

void dongle_unlock(t_dongle * dongle, int cd_time)
{
    if (dongle)
	{
		usleep(converter((uint64_t)cd_time));
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
