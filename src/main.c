/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vs <vs@student.42.fr>                        +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/07/15 18:23:43 by vsudak        #+#    #+#                 */
/*   Updated: 2026/07/20 13:10:07 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


# include "codexion.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void	*work(void *smth)
{
    int i;
	t_state	*flow[3];
	t_state code;
	t_state debug;
	t_state	refactor;

	code = COMPILE;
	debug = DEBUG;
	refactor = REFACTOR;
	*flow = {&code, &debug, &refactor};
    i = 0;
	while (i < 3)
    {
        printf("%s\n", flow[i]);
    }
}

int main()
{
    pthread_t coder;
    int id;
    void *smth = NULL;

    id = pthread_create(&coder, NULL, (void *)work(NULL), smth);
	pthread_join(id, NULL);
	printf("\nthread joined to the main one\n");
    return 0;
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
	


