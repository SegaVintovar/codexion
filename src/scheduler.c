# include "codexion.h"


t_coder *fifo()
{
    // implement queue, make struct
}


t_coder *edf()
{
    // sort them by the time to burnout
}

// returns us the coder that should be the next one to pick up the dongle
t_coder *scheduler(t_quantum_compiler *instance)
{
    if (instance->scheduler == FIFO)
    {
        return (fifo());
    }
    else if (instance->scheduler == EDF)
    {
        return (edf());
    }
}
