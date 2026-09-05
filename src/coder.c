#include "codexion.h"

t_coder *new_coder(int id, t_quantum_compiler *state)
{
    t_coder     *new;

    new = malloc(sizeof(t_coder));
    if (!new)
        return NULL;
    new->id = id;
    new->compiles_left = state->comp_c_r;
    new->state = state;
	new->last_comp_t = 0;
    return (new);
}

// c 0: d0, d2; c 1: d0, d1; c 2: d1, d2
// d 0 d 1 d 2

void    assign_dongles(t_coder *coder, t_quantum_compiler *state)
{
    int c_id;
    int ld_id;
    int rd_id;

    c_id = coder->id;
    if (c_id == 0)
    {
        ld_id = state->coders_c - 1;
        rd_id = c_id;
    }
    else if (c_id == state->coders_c - 1)
    {
        ld_id = c_id - 1;
        rd_id = c_id;
    }
    else
    {
        ld_id = c_id - 1;
        rd_id = c_id;
    }
    coder->left = state->dongles[ld_id];
    coder->right = state->dongles[rd_id];   
}  
