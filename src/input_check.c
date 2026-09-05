/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vsudak <vsudak@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/09/05 13:10:10 by vsudak        #+#    #+#                 */
/*   Updated: 2026/09/05 15:22:43 by vsudak        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int ft_isdigit(int c)
{
	if(c >= 48 && c <= 57)
		return(1);
	else
		return(0);
}

int is_scheldue(char *arg)
{
    if (!strcmp(arg, "edf") || !strcmp(arg, "fifo"))
    {
        return (1);
    }
    else
        return (0);
}

// sort of isnumber
int	isint(char *arg)
{
	size_t	len;
	size_t	p;

	len = strlen(arg);
	if (len < 1)
		return (0);
	p = 0;
	while (arg[p] == ' ' || (arg[p] >= 9 && arg[p] <= 13))
		p++;
	if (arg[p] == '+' || arg[p] == '-')
		p++;
	while (p < len)
	{
		if (ft_isdigit((int)arg[p]))
			p++;
		else
			return (0);
	}
	return (1);
}

// check if all the args are under INT_MAX
int	int_max_and_positivity_check(int argc, char **argv)
{
	long	tmp;
	size_t	i;

	i = 1;
	while (i < argc - 2)
	{
		tmp = my_atoi(argv[i]);
		if (tmp > INT_MAX || tmp < 0)
        {
            printf("int max test\n"); return (0);
        }
        i++;
	}
	return (1);
}


int input_check(int argc, char **argv)
{
    int i;

    i = 1;
    while (i < argc - 1)
    {
		if (!isint(argv[i]))
        {
			printf("isint FAIL\n");
			return 0;
        }
        i++;
    }
	if (!int_max_and_positivity_check(argc, argv))
    {
        printf("int max and positivity FAIL exit\n");
		return 0;
    }
    return 1;
}
