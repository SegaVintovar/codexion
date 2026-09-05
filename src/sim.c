#include "codexion.h"

uint64_t    converter(uint64_t t)
{
    return (t * 1000);
}

uint64_t    curtime_full()
{
    struct timeval		curtime;
	uint64_t			curtime_full;

	gettimeofday(&curtime, NULL);
	curtime_full = (curtime.tv_sec * (uint64_t)1000) + (curtime.tv_usec / 1000);
    return (curtime_full);
}

uint64_t    time_scince_start(t_quantum_compiler *state)
{
    // uint64_t    curtime_f;
    uint64_t    result;

    result = curtime_full() - state->start_time;
    // curtime_f = curtime_full();
    return (result);
}

void    compiling(t_coder *coder, t_quantum_compiler *state)
{
    // check if there are any compiles left to do and
	// if so, then ->
	// check the time since last compile
    uint64_t    t;
    // milisecnds into microseconds to pass it to thr usleep
	if (coder->compiles_left)
	{
		// t = time_scince_start(state);
		// first start
		if (coder->compiles_left == state->comp_c_r)
		{
			// pass
			// coder->last_comp_t = curtime_full();
			if ((curtime_full() - state->start_time) > state->burnout_t) // ->
                pthread_cond_signal(&state->burnoutSignal); // we got burnout
		}
		else if ((curtime_full() - coder->last_comp_t) > state->burnout_t && \
			coder->compiles_left != state->comp_c_r)
		{
			// we got burnout
            pthread_cond_signal(&state->burnoutSignal);
			// sent signal to monitor so it will broadcast to all
		}
		dongle_lock(&coder->right->mutex, coder->id, state->start_time);
		dongle_lock(&coder->left->mutex, coder->id, state->start_time);
		coder->last_comp_t = curtime_full();
        t = coder->last_comp_t - state->start_time;
		printf("%lu %i has started compiling\n", t, coder->id);

		usleep(converter((uint64_t)state->compile_t));
		coder->compiles_left--;

        dongle_unlock(coder->left, state->dongle_cd);
        dongle_unlock(coder->right, state->dongle_cd);
	}
}

void    refactoring(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;

    t = time_scince_start(state);
    printf("%lu %i has started refactoring\n", t, coder->id);
    usleep(converter((uint64_t)state->refactor_t));
    
}

void    debugging(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;

    t = time_scince_start(state);
    printf("%lu %i has started debugging\n", t, coder->id);
    usleep(converter((uint64_t)state->debug_t));
}

// void *simulation(t_coder *coder, t_quantum_compiler *state)
void *simulation(void *coder)
{
    int                 i;
    t_coder             *c;
    t_quantum_compiler  *state;

    c =(t_coder *)coder;
    i = 0;
    printf("Sim Start %i\n", c->id);
    // 
    while (i < c->state->comp_c_r) // and there is no burnout signal
    {
        compiling(c, c->state);
        refactoring(c, c->state);
        debugging(c, c->state);
        i++;
    }
    return NULL;
}


void run(t_quantum_compiler *state)
{
    int             i;
    t_coder         *c;
    // t_thread_args   *args;

    // args = setup_args(state);
    // if (!args)
    //     return;
    state->start_time = curtime_full();
    i = 0;
    while (i < state->coders_c)
    {
        // args- = state->coders[i];
        c = state->coders[i];
        pthread_create(&state->coders[i]->thread, NULL, simulation, (void *)c);
        i++;
    }
    start_monitor(state);
	// start monitor
    i = 0;
    while (i < state->coders_c)
    {
        c = state->coders[i];
        pthread_join(c->thread, NULL);
        i++;
    }
    stop_monitor(state);
	// stop monitor
}
