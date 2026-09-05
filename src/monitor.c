#include "codexion.h"

int check_thread(t_coder *coder, t_quantum_compiler *state)
{
	uint64_t now;

	now = curtime_full();
	if ((now - coder->last_comp_t) > state->burnout_t)
		return 1;
	else
		return 0;
}

void	*monitor(void *arg)
{
	int sig;
	int i;
	t_quantum_compiler *state;

	state = (t_quantum_compiler *)arg;
	i = 0;
	sig = 1;
	while (sig)
	{
		if (i == state->coders_c)
			i = 0;
		if (check_thread(state->coders[i], state))
			return ((void *)1);
		i++;
	}
	return (NULL);
}

void	start_monitor(t_quantum_compiler *state)
{
	// pthread_t	*mon_tr;
	pthread_create(&state->monitor_tread, NULL, monitor, (void *)state);	
}

void    stop_monitor(t_quantum_compiler *state)
{
    pthread_join(state->monitor_tread, NULL);
}