/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   safePrint.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/07 17:10:05 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/26 15:40:54 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// new print for burnout report

void	safePrint(t_quantum_compiler *state, t_coder *coder, char *stage)
{
	uint64_t	t;

	// if not burned then print
	// pthread_mutex_lock(&state->print_m);
	if (burnoutReportCheck(state))
	{
		// pthread_mutex_unlock(&state->print_m);
		return ;
	}
	pthread_mutex_lock(&state->burnoutMutex);
	t = curtime_full() - state->start_time;
    printf("%lu %i has %s\n", t, (coder->id + 1), stage); // coder id + 1
	pthread_mutex_unlock(&state->burnoutMutex);
}
