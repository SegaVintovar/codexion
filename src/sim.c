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
    
    // curtime_f = curtime_full();
    return (curtime_full() - state->start_time);
}

void    compiling(t_coder *coder, t_quantum_compiler *state)
{
    // check the time since last compile
    uint64_t    t;
    // milisecnds into microseconds to pass it to thr usleep
    t = time_scince_start(state);
    // first start
    if (coder->compiles_left == state->comp_c_r)
    {
        // pass
        if (t > state->burnout_t){
            // we got burnout
        }
    }
    else if ((curtime_full() - coder->last_comp_t) > state->burnout_t && \
        coder->compiles_left != state->comp_c_r)
    {
        // we got burnout
        // send signal to stop all
    }
    // take dongles + update the time since last compile
    pthread_mutex_lock(&coder->left->mutex);
    pthread_mutex_lock(&coder->right->mutex);
    coder->last_comp_t = curtime_full();
    printf("%lu - %i has taken a dongle and started compiling\n", t, coder->id);

    usleep(converter((uint64_t)state->compile_t));
    coder->compiles_left--;

    pthread_mutex_unlock(&coder->left->mutex);
    pthread_mutex_unlock(&coder->right->mutex);
}

void    refactoring(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;

    t = time_scince_start(state);
    printf("%lu - %i has started refactoring\n", t, coder->id);
    usleep(converter((uint64_t)state->refactor_t));
    
}

void    debugging(t_coder *coder, t_quantum_compiler *state)
{
    uint64_t    t;

    t = time_scince_start(state);
    printf("%lu - %i has started debugging\n", t, coder->id);
    usleep(converter((uint64_t)state->debug_t));
}

// void *simulation(t_coder *coder, t_quantum_compiler *state)
void *simulation(void *argumnets)
{
    int                 i;
    t_coder             *coder;
    t_quantum_compiler  *state;
    t_thread_args       *args;

    args = (t_thread_args *)argumnets;  // could not cast...
    // coder = args->coder;
    // state = args->state;
    i = 0;
    printf("Sim Start %i\n", args->coder->id);
    args->state->start_time = curtime_full();
    while (i < args->state->comp_c_r) // and there is no burnout signal
    {
        compiling(args->coder, args->state);
        refactoring(args->coder, args->state);
        debugging(args->coder, args->state);
        i++;
    }
    return NULL;
}

void run(t_quantum_compiler *state)
{
    int                 i;
    t_coder             *c;
    t_thread_args       *args;

	args = malloc(sizeof(t_thread_args));
	if (!args)
		return;
    args->state = state;
    i = 0;
	
    while (i < state->coders_c)
    {
        args->coder = state->coders[i];
        c = state->coders[i];
        pthread_create(&state->coders[i]->thread, NULL, simulation, (void *)&args);
        i++;
    }
	// start monitor
    i = 0;
    while (i < state->coders_c)
    {
        c = state->coders[i];
        pthread_join(c->thread, NULL);
        i++;
    }
	// stop monitor
	if (args)
		free(args);
}
