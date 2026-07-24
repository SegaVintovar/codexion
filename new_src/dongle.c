# include "codexion.h"

t_dongle	*dongle_new(int id)
{
    t_dongle    *new;

    new = malloc(sizeof(t_dongle));
    if  (!new)
        return NULL;
    new->locked = false;
    new->id = id;
    return new;
}

void dongle_lock(t_dongle *dongle)
{
    if (dongle)
        dongle->locked = true;
}

void dongle_unlock(t_dongle * dongle)
{
    if (dongle)
        dongle->locked = false;
}

void free_dongle(t_dongle *dongle)
{
    if (dongle)
        free(dongle);
}

void dongle_cd(t_dongle *dongle)
{
    usleep(1000);
}