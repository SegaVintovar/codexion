/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 12:44:05 by vs                #+#    #+#             */
/*   Updated: 2026/09/29 14:38:35 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// returns address of the memory where we will store two pointers of our queue
t_coder	**init_queue(void)
{
	t_coder	**result;

	result = malloc(sizeof(t_coder *) * 2);
	if (!result)
		return (NULL);
	result = (t_coder **)memset((void *)result, 0, sizeof(t_coder *) * 2);
	return (result);
}

void	destroy_queue(t_coder **queue)
{
	free(queue);
}
