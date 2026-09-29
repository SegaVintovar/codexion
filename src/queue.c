/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   queue.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: vs <vs@student.42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/09/14 18:16:17 by vsudak			#+#	#+#			 */
/*   Updated: 2026/09/29 12:35:37 by vs			   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_coder **queue)
{
	t_coder	*tmp;

	tmp = queue[0];
	queue[0] = queue[1];
	queue[1] = tmp;
}

void	edf_insert(t_dongle *dongle, t_coder *first, t_coder *second)
{
	uint64_t	a;
	uint64_t	b;

	a = 0;
	b = 0;
	pthread_mutex_lock(&first->state->last_comp_t_mutex);
	if (first)
		a = first->last_comp_t;
	if (second)
		b = second->last_comp_t;
	pthread_mutex_unlock(&first->state->last_comp_t_mutex);
	if (first && second && a > b)
		swap(dongle->queue);
}

// add coder to the queue
void	enque(t_dongle *dongle, t_coder *coder)
{
	t_coder	*first;
	t_coder	*second;

	first = NULL;
	second = NULL;
	pthread_mutex_lock(&dongle->queue_mutex);
	if (dongle->queue[0])
		dongle->queue[1] = coder;
	else
		dongle->queue[0] = coder;
	first = dongle->queue[0];
	second = dongle->queue[1];
	if (coder->state->scheduler != EDF)
		return ((void)pthread_mutex_unlock(&dongle->queue_mutex));
	edf_insert(dongle, first, second);
	pthread_mutex_unlock(&dongle->queue_mutex);
}

void	pop(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->queue_mutex);
	dongle->queue[0] = dongle->queue[1];
	dongle->queue[1] = NULL;
	pthread_mutex_unlock(&dongle->queue_mutex);
}
