/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:11:40 by vs                #+#    #+#             */
/*   Updated: 2026/09/29 14:36:54 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// new print for burnout report
void	safe_print(t_quantum_compiler *state, t_coder *coder, char *stage)
{
	uint64_t	t;

	pthread_mutex_lock(&state->burnout_mutex);
	if (state->burnout_reported)
	{
		pthread_mutex_unlock(&state->burnout_mutex);
		return ;
	}
	t = curtime_full() - state->start_time;
	printf("%lu %i has %s\n", t, (coder->id + 1), stage);
	pthread_mutex_unlock(&state->burnout_mutex);
}
