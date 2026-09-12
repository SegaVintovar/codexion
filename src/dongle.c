/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dongle.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:00:44 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/12 15:55:40 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

t_dongle	*dongle_new(int id)
{
    t_dongle    *new;
    t_coder     *queue[2];

    new = malloc(sizeof(t_dongle));
    if  (!new)
        return NULL;
    // new->locked = false;
    new->id = id;
    new->avaliable_at = 0;
    new->queue = queue;
    if (!new->queue)
    {
        free(new);
        return (NULL);
    }
    pthread_mutex_init(&new->mutex, NULL);
    return new;
}

// not in use anymore
// void dongle_lock(t_dongle *dongle, t_coder *coder)
// {
//     uint64_t    now;
//     uint64_t    time2sleep;
//     int         was_locked;
    
//     time2sleep = 0;
//     if (dongle)
//     {
//         was_locked = 0;
// 		pthread_mutex_lock(&dongle->mutex);
//         if (burnoutCheck(coder->state, coder))
// 		{
// 			pthread_mutex_unlock(&dongle->mutex);
// 			return ;
// 		}
//         was_locked = 1;
//         now = curtime_full();
//         if (dongle->avaliable_at > now)
//         {
//             time2sleep = dongle->avaliable_at - now;
//             if (dongle->avaliable_at - coder->last_comp_t > (uint64_t)coder->state->burnout_t)
//             {
//                 burnoutReport(coder->state, coder, (uint64_t)0);
//                 return ((void)pthread_mutex_unlock(&dongle->mutex));
//             }
//         }
//         if (time2sleep != 0)
//         {
//             dongle_unlock(dongle);
//             was_locked = 0;
//             usleep(converter(time2sleep));
//         }
//         if (!was_locked)
//             pthread_mutex_lock(&dongle->mutex); 
//     }
// }

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
