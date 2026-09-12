#include "codexion.h"

int simIsFinished(t_coder *coder)
{
    
    if (coder->state->coders_c == coder->state->codersFinished)
        return (1);
    else
        return (0);
}
// check if coder is burnedout 
// I dont report here!!!
// or not finished
int isBurned(t_quantum_compiler *state, t_coder *coder)
{
    uint64_t    now;
	uint64_t	last_comp;
	// int 		bo;

	// bo = 0;

	pthread_mutex_lock(&coder->state->state_mutex);
    if ((coder->state->coders_c == coder->state->codersFinished))
        {
            pthread_cond_signal(&state->burnoutSignal);
            pthread_mutex_unlock(&coder->state->state_mutex);
            return (1);
        }
	now = curtime_full();
	last_comp = coder->last_comp_t;
	// bo = coder->state->burnoutReported;
	pthread_mutex_unlock(&coder->state->state_mutex);
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
