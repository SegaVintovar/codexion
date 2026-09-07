/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   safePrint.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/07 17:10:05 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/07 17:50:33 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	safePrint(t_quantum_compiler *state, t_coder *coder, char *stage)
{
	uint64_t    	t;
	
	pthread_mutex_lock(&state->print_m);
	t = curtime_full() - state->start_time;
    printf("%lu %i has %s\n", t, coder->id, stage);
	pthread_mutex_unlock(&state->print_m);
}