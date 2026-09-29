/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_acquisition.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:22:48 by vs                #+#    #+#             */
/*   Updated: 2026/09/29 15:07:21 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_next(t_coder *coder, t_dongle *first, t_dongle *second)
{
	int	is_next;

	is_next = 0;
	pthread_mutex_lock(&first->queue_mutex);
	pthread_mutex_lock(&second->queue_mutex);
	if (first->queue[0] == coder && second->queue[0] == coder)
		is_next = 1;
	pthread_mutex_unlock(&first->queue_mutex);
	pthread_mutex_unlock(&second->queue_mutex);
	return (is_next);
}

// here we are getting rid of deadlock
// basically first coder takes dongles in different order
void	one_two_mutex(t_coder *coder, t_dongle **first, t_dongle **second)
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

void	request(t_coder *coder, t_dongle *first, t_dongle *second)
{
	enque(first, coder);
	enque(second, coder);
	while (is_next(coder, first, second) == 0)
		usleep(500);
}

void	grab_two_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	int	burned;

	burned = grab_dongle(first, coder);
	pop(first);
	if (burned)
	{
		if (second != first)
			pop(second);
		return ;
	}
	if (second != first)
	{
		burned = grab_dongle(second, coder);
		pop(second);
		if (burned)
		{
			pthread_mutex_unlock(&first->mutex);
			return ;
		}
	}
	else
		usleep(converter(coder->state->burnout_t));
}

int	dongle_acquisition(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	one_two_mutex(coder, &first, &second);
	if (first == second)
	{
		usleep(converter(coder->state->burnout_t));
		return (1);
	}
	request(coder, first, second);
	grab_two_dongles(coder, first, second);
	if (is_burned(coder->state, coder) == 1)
		return (drop_dongles(coder), 1);
	return (0);
}
