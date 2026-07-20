#include "codexion.h"

pthread_create(); // thread, NULL, working func, arguments for this function -> starts the thread and returns 0 on success
pthread_join(); // thread, return value(NULL) -> waits for the thread to finish the job
pthread_mutex_init(); // pthread_mutex_t, 
pthread_mutex_lock();
pthread_mutex_unlock();
pthread_mutex_destroy();
pthread_cond_init();
pthread_cond_wait();
pthread_cond_timedwait();
pthread_cond_signal();
pthread_cond_broadcast();
pthread_cond_destroy();