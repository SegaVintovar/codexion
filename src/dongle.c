# include "codexion.h"

t_dongle	*dongle_new(int id)
{
    t_dongle    *new;

    new = malloc(sizeof(t_dongle));
    if  (!new)
        return NULL;
    // new->locked = false;
    new->id = id;
    pthread_mutex_init(&new->mutex, NULL);
    return new;
}

void dongle_lock(pthread_mutex_t *dongle, int coder_id, uint64_t start_time)
{
    uint64_t    t;

    if (dongle)
	{
		pthread_mutex_lock(dongle);
        t = curtime_full() - start_time;
		printf("%lu %i has taken a dongle\n", t, coder_id);
	}
}

void dongle_unlock(t_dongle * dongle, int cd_time)
{
    if (dongle)
        // dongle->locked = false;
        pthread_mutex_unlock(&dongle->mutex);
        usleep(converter((uint64_t)dongle->));
}

// this one will go into free all
void free_dongle(t_dongle *dongle)
{
    if (dongle)
    {
        pthread_mutex_destroy(&dongle->mutex);
        free(dongle);
    }
}

void dongle_cd(t_dongle *dongle, uint64_t time2sleep){
    usleep(time2sleep);
}
