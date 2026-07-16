# include "codexion.h"

t_quantum_compiler	*init(int argc, char **argv)
{
	int					i;
	t_quantum_compiler	*result;
	t_scheduler			type;

	if (argv[8] == "fifo")
		type = FIFO;
	else if (argv[8] == "edf")
		type = EDF;
	result = malloc(sizeof(t_quantum_compiler));
	if (!result)
		return NULL;
	// do some checks here
	result->coders_c = atoi(argv[1]);
	result->burnout_t = atoi(argv[2]);
	result->compile_t = atoi(argv[3]);
	result->debug_t = atoi(argv[4]);
	result->refactor_t = atoi(argv[5]);
	result->comp_c_r = atoi(argv[6]);
	result->dongle_cd = atoi(argv[7]);
	result->scheduler = type;

	return (result);
}
