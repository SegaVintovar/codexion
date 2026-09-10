#include "codexion.h"

// check if coder is burnedout or will be burned before next action
int isBurned(t_quantum_compiler *state, t_coder *coder, uint64_t time_till_next_action)
{
    uint64_t    now;
    uint64_t    marge;

    now = curtime_full();
    // if it has happend
    if (coder->last_comp_t + state->burnout_t > now)
    {
        burnoutReport(state, coder, (uint64_t)0);
        return (1);
    }
    // if it will happen
    if (now - coder->last_comp_t + time_till_next_action > state->burnout_t)
    {
        marge = ;
        burnoutReport(state, coder, )
    }
}