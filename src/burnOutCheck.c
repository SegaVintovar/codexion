#include "codexion.h"

// check if coder is burnedout 
// I dont report here!!!
int isBurned(t_quantum_compiler *state, t_coder *coder)
{
    uint64_t    now;
	uint64_t	last_comp;
	// int 		bo;

	// bo = 0;
	pthread_mutex_lock(&coder->time_check);
	now = curtime_full();
	last_comp = coder->last_comp_t;
	// bo = coder->state->burnoutReported;
	pthread_mutex_unlock(&coder->time_check);
    // if it has happend
    if (now - last_comp >= (uint64_t)state->burnout_t)
		return (1);
	else
	{
		pthread_mutex_lock(&state->burnoutMutex);
		if (state->burnoutReported == 1)
			return (pthread_mutex_unlock(&state->burnoutMutex), 1);
		return (pthread_mutex_unlock(&state->burnoutMutex), 0);
	}
}

// here we are reporting about burnout(stop)
void    burnoutReport(t_quantum_compiler *state, t_coder *coder, uint64_t whenWeGotBurned)
{
    pthread_mutex_lock(&state->burnoutMutex);
    state->burnoutReported = 1;
    state->whoGotBurned = coder->id;
	if (whenWeGotBurned == 0)
    	state->whenWeGotBurn = curtime_full() - state->start_time;
    else
		state->whenWeGotBurn = whenWeGotBurned;
	pthread_cond_signal(&state->burnoutSignal); // we got burnout
    pthread_mutex_unlock(&state->burnoutMutex);
}

// here we are checking if burnout was already reported
int burnoutReportCheck(t_quantum_compiler *state)
{
    int result;
    
    pthread_mutex_lock(&state->burnoutMutex);
    result = state->burnoutReported;
    pthread_mutex_unlock(&state->burnoutMutex);
    return (result);
}

// For compiling top check if current coder is not burnedout
int	burnoutCheck(t_quantum_compiler *state, t_coder *coder)
{
	if (!coder->compiles_left || burnoutReportCheck(state))
        return (1);
    if (coder->compiles_left == state->comp_c_r) // first start
    {
        if ((curtime_full() - state->start_time) > (uint64_t)state->burnout_t) // ->
        {
            burnoutReport(state, coder, 0);
            return (1);
        }
	}
    else if ((curtime_full() - coder->last_comp_t) > \
        (uint64_t)state->burnout_t && \
        coder->compiles_left != state->comp_c_r)
    {
        burnoutReport(state, coder, 0);
		return (1);
    }
	return (0);
}
