#include "codexion.h"

void    destroy_dongles(t_sim *sim)
{
    int i;
    int n;

    if (!sim->dongles)
        return ;
    n = sim->config.number_of_coders;
    i = 0;
    while (i < n)
    {
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        pthread_cond_destroy(&sim->dongles[i].cond);
        i++;
    }
}

void    destroy_sim(t_sim *sim)
{
    if (!sim)
        return ;

    /* dongles */
    destroy_dongles(sim);
    free(sim->dongles);
    sim->dongles = NULL;

    /* coders */
    free(sim->coders);
    sim->coders = NULL;

    /* sim mutexes */
    pthread_mutex_destroy(&sim->log_mutex);
    pthread_mutex_destroy(&sim->stop_mutex);
}
