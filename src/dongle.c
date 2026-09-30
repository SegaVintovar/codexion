/* ************************************************************************** */
/*									  */
/*							:::	 ::::::::   */
/*   dongle.c								 :+:   :+:	:+:   */
/*						  +:+ +:+	   +:+	*/
/*   By: vs <vs@student.42.fr>			+#+  +:+	   +#+	*/
/*						+#+#+#+#+#+   +#+	 */
/*   Created: 2026/09/05 13:00:44 by vsudak   #+#  #+#		  */
/*   Updated: 2026/09/29 12:25:11 by vs	  ###   ########.fr	*/
/*									  */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*dongle_new(int id)
{
	t_dongle	*new;

	new = malloc(sizeof(t_dongle));
	if (!new)
		return (NULL);
	new->id = id;
	new->avaliable_at = 0;
	new->buzy = 0;
	pthread_mutex_init(&new->mutex, NULL);
	pthread_mutex_init(&new->queue_mutex, NULL);
	new->queue = init_queue();
	if (!new->queue)
	{
		pthread_mutex_destroy(&new->mutex);
		free(new);
		return (NULL);
	}
	return (new);
}

// this one will go into free all
void	free_dongle(t_dongle *dongle)
{
	if (dongle)
	{
		pthread_mutex_destroy(&dongle->mutex);
		pthread_mutex_destroy(&dongle->queue_mutex);
		free(dongle);
	}
}

void	drop_dongles(t_coder *coder)
{
	uint64_t	avail_at;

	avail_at = curtime_full() + coder->state->dongle_cd;
	release_dongle(coder->left, avail_at);
	if (coder->left != coder->right)
		release_dongle(coder->right, avail_at);
}

int	grab_dongle(t_dongle *first, t_coder *coder)
{
	uint64_t	avail_at;
	uint64_t	now;

	if (claim_dongle(first, coder, &avail_at))
		return (1);
	now = curtime_full();
	if (avail_at > now)
		usleep(converter(cap_to_deadline(coder, avail_at - now)));
	if (is_burned(coder->state, coder) == 1)
		return (release_dongle(first, 0), 1);
	safe_print(coder->state, coder, "taken dongle");
	return (0);
}
