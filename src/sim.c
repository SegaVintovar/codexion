/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:00:53 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/07 11:09:21 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    burnoutReport(t_quantum_compiler *state, t_coder *coder)
{
    pthread_mutex_lock(&state->burnoutMutex);
    state->burnoutReported = 1;
    state->whoGotBurned = coder->id;
    state->whenWeGotBurn = curtime_full() - state->start_time;
    pthread_cond_signal(&state->burnoutSignal); // we got burnout
    pthread_mutex_unlock(&state->burnoutMutex);
}

int burnoutReportCheck(t_quantum_compiler *state)
{
    atomic_int result;
    
    pthread_mutex_lock(&state->burnoutMutex);
    result = state->burnoutReported;
    pthread_mutex_unlock(&state->burnoutMutex);
    return ((int)result);
}

void    compiling(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;
    // int         reported;

    // reported = burnoutReportCheck(state);

	if (!coder->compiles_left || burnoutReportCheck(state))
        return ;
    if (coder->compiles_left == state->comp_c_r) // first start
    {
        if ((curtime_full() - state->start_time) > (uint64_t)state->burnout_t) // ->
            burnoutReport(state, coder);
    }
    else if ((curtime_full() - coder->last_comp_t) > \
        (uint64_t)state->burnout_t && \
        coder->compiles_left != state->comp_c_r)
    {
        burnoutReport(state, coder);
    }
    if (!burnoutReportCheck(state))
        dongle_lock(&coder->right->mutex, coder->id, state->start_time);
    else
        return;
    if (!burnoutReportCheck(state))
        dongle_lock(&coder->left->mutex, coder->id, state->start_time);
    else
        return;
    coder->last_comp_t = curtime_full();
    t = coder->last_comp_t - state->start_time;
    printf("%lu %i has started compiling\n", t, coder->id);
    usleep(converter((uint64_t)state->compile_t));
    coder->compiles_left--;
    dongle_unlock(coder->left, state->dongle_cd);
    dongle_unlock(coder->right, state->dongle_cd);
}

void    refactoring(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;

    t = time_scince_start(state);
    printf("%lu %i has started refactoring\n", t, coder->id);
    usleep(converter((uint64_t)state->refactor_t));
    
}

void    debugging(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;

    t = time_scince_start(state);
    printf("%lu %i has started debugging\n", t, coder->id);
    usleep(converter((uint64_t)state->debug_t));
}

// void *simulation(t_coder *coder, t_quantum_compiler *state)
void *simulation(void *coder)
{
    int                 i;
    t_coder             *c;

    c =(t_coder *)coder;
    i = 0;
    // printf("Sim Start %i\n", c->id);
    // 
    while (i < c->state->comp_c_r && !burnoutReportCheck(c->state)) // and there is no burnout signal
    {
        compiling(c, c->state);
        if (burnoutReportCheck(c->state))
            break;
        refactoring(c, c->state);
        if (burnoutReportCheck(c->state))
            break;
         debugging(c, c->state);
        i++;
    }
    return NULL;
}

void run(t_quantum_compiler *state)
{
    int             i;
    t_coder         *c;

    state->start_time = curtime_full();
	start_monitor(state);
    i = 0;
    while (i < state->coders_c)
    {
        // args- = state->coders[i];
        c = state->coders[i];
        pthread_create(&state->coders[i]->thread, NULL, simulation, (void *)c);
        i++;
    }
    i = 0;
    while (i < state->coders_c)
    {
        c = state->coders[i];
        pthread_join(c->thread, NULL);
        i++;
    }
    stop_monitor(state);
}
