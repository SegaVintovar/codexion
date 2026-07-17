/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/07/15 18:23:43 by vsudak        #+#    #+#                 */
/*   Updated: 2026/07/17 17:28:21 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


# include "codexion.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void	my_func(void *smth)
{
	
}

int main(int argc, char **argv)
{
	int i;
    pthread_t   desc;
    int ptc;
    const pthread_attr_t *idk;

    ptc = pthread_create(&desc, idk, my_func(), );
    if (ptc == 0)
        run_the_program(whatever);
    else
    {
        if (ptc == EAGAIN)
    }
	if (argc != 9)
	{
		exit(1);
	}
	

}
