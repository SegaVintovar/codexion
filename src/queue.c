/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:16:17 by vsudak            #+#    #+#             */
/*   Updated: 2026/09/20 13:45:04 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// returns address of the memory where we will store two pointers of our queue
t_coder **initQueue()
{
    t_coder **result;
    
    result = malloc(sizeof(t_queue *) * 2);
    if (!result)
        return (NULL);
    result = (t_coder **)memset((void *)result, 0, sizeof(t_coder *) * 2);
    return (result);
}

void    destroyQueue(t_coder **queue)
{
    free(queue);
}

void    swap(t_coder **queue)
{
    t_coder *tmp;
    
    tmp = queue[0];
    queue[0] = queue[1];
    queue[1] = tmp;
}

// add coder to the queue
void enque(t_dongle *dongle, t_coder *coder)
{
    pthread_mutex_lock(&dongle->mutex);
    if (dongle->queue[0])
        dongle->queue[1] = coder;
    else
        dongle->queue[0] = coder;
    pthread_mutex_unlock(&dongle->mutex);

    
    if (coder->state->scheduler != EDF)
        return ;
    // {
    //     // sort of heapify
    //     pthread_mutex_lock(&coder->state->last_comp_t_mutex);
    //     if (dongle->queue[0] && dongle->queue[1])
    //         if (dongle->queue[0]->last_comp_t > dongle->queue[1]->last_comp_t)
    //             swap(dongle->queue);

    //     pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
        // if (dongle->queue[0] && !dongle->queue[1])
    uint64_t	a;
    uint64_t	b;

    a = 0;
    b = 0;
    pthread_mutex_lock(&coder->state->last_comp_t_mutex);
    if (dongle->queue[0])
        a = dongle->queue[0]->last_comp_t;
    pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
    pthread_mutex_lock(&coder->state->last_comp_t_mutex);
    if (dongle->queue[1])
        b = dongle->queue[1]->last_comp_t;
    pthread_mutex_unlock(&coder->state->last_comp_t_mutex);
    if (a > b)
        swap(dongle->queue);
}
    // pthread_mutex_unlock(&dongle->mutex);


void pop(t_dongle *dongle)
{
    t_coder *tmp;
    
    tmp = NULL;
    // pthread_mutex_lock(&dongle->mutex);
    if (dongle->queue[1])
        tmp = dongle->queue[1];
    // how to be sure that I am not working with memory where the coder is allocated
    // dongle->queue = memset((void *)dongle->queue, 0, sizeof(t_coder *) * 2);
    dongle->queue[0] = tmp;
    dongle->queue[1] = NULL;
    // pthread_mutex_unlock(&dongle->mutex);
    // return result;
}
