/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 15:04:25 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/05 15:15:04 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

uint64_t    converter(uint64_t t)
{
	return (t * 1000);
}


uint64_t    curtime_full()
{
    struct timeval		curtime;
	uint64_t			curtime_full;

	gettimeofday(&curtime, NULL);
	curtime_full = (curtime.tv_sec * (uint64_t)1000) + (curtime.tv_usec / 1000);
    return (curtime_full);
}

uint64_t    time_scince_start(t_quantum_compiler *state)
{
    uint64_t    result;

    result = curtime_full() - state->start_time;
    return (result);
}
