/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_creation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:34:53 by vs                #+#    #+#             */
/*   Updated: 2026/09/29 13:40:00 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	thread_creation_fail(t_quantum_compiler *state, int i)
{
	while (i)
	{
		pthread_join(state->coders[i]->thread, NULL);
		i--;
	}
}

// I need to check pthread create return value and I need to handle it
int	start_batch_of_coders(t_quantum_compiler *state, int batch)
{
	int		i;
	t_coder	*c;

	i = 0;
	while (i < state->coders_c)
	{
		if (i % 2 == batch)
		{
			c = state->coders[i];
			if (pthread_create(&c->thread, NULL, sim, (void *)c))
			{
				thread_creation_fail(state, i);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	join_first_batch(t_quantum_compiler *state)
{
	int	i;

	i = 0;
	while (i < state->coders_c)
	{
		if (i % 2 == 0)
			pthread_join(state->coders[i]->thread, NULL);
		i++;
	}
}
