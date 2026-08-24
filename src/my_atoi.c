/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:27:39 by vs                #+#    #+#             */
/*   Updated: 2026/07/24 10:35:52 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// to handle the int overflow i ll stop conversion as soon as my long nbr
// going to be bigger then INT_MAX, so the return woudl be
// in the long - int range
long	my_atoi(const char *nptr)
{
	int		i;
	long	nbr;
	int		mp;

	i = 0;
	mp = 1;
	nbr = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			mp = mp *(-1);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = nbr * 10 +(nptr[i] - '0');
		if (nbr > INT_MAX)
			break ;
		i++;
	}
	return (nbr * mp);
}