#include "codexion.h"

/* Read stop safely */
int sim_should_stop(t_sim *sim)
{
    pthread_mutex_lock(&sim->stop_mutex);
    int v = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);
    return v;
}

/* Write stop safely */
void sim_set_stop(t_sim *sim, int v)
{
    pthread_mutex_lock(&sim->stop_mutex);
    sim->stop = v;
    pthread_mutex_unlock(&sim->stop_mutex);
}

/*
 * Print a state line safely (no mixed lines).
 * Format must match subject:
 * timestamp X <msg>
 */
void log_state(t_sim *sim, int id, const char *msg)
{
    long ts;

    if (sim_should_stop(sim))
        return;
    pthread_mutex_lock(&sim->log_mutex);
    ts = now_ms() - sim->start_time;
    printf("%ld %d %s\n", ts, id, msg);
    pthread_mutex_unlock(&sim->log_mutex);
}

/*
 * Burnout must always print (even if stop becomes 1).
 */

void log_burnout(t_sim *sim, int id)
{
    long ts;

    ts = now_ms() - sim->start_time;
    pthread_mutex_lock(&sim->log_mutex);
    printf("%ld %d burned out\n", ts, id);
    pthread_mutex_unlock(&sim->log_mutex);
}
