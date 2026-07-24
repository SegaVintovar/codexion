#include "codexion.h"

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
