/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vs <vs@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 18:23:43 by vsudak            #+#    #+#             */
/*   Updated: 2026/07/17 18:53:19 by vs               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "codexion.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void	work(void *smth)
{
    int i;

    i = 0;
	while (i < 3)
    {
        printf("%s\n", t_state[i]);
    }
}

int main()
{
    pthread_t coder;
    int id;
    void *smth

    pthread_create(&coder, NULL, work(), smth)
    
}

// int main(int argc, char **argv)
// {
// 	int i;
//     pthread_t   desc;
//     int ptc;
//     const pthread_attr_t *idk;
//     t_quantum_compiler  *instance;

//     if (argc != 9)
//     {
//         printf("Argument count is incorrect");
//         exit(1);
//     }
//     instance = init_compiler(argc, argv);
//     if (!instance)
//         exit(1);
//     start(instance);
    
    
    
    
    // ptc = pthread_create(&desc, idk, my_func(), );
    // if (ptc == 0)
    //     run_the_program(whatever);
    // else
    // {
    //     if (ptc == EAGAIN)
    // }
	// if (argc != 9)
	// {
	// 	exit(1);
	// }
	


