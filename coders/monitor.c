#include "codexion.h"

static void wake_all_dongles(t_sim *sim)
{
    int j;
    int n;

    if (!sim->dongles)
        return;
    n = sim->config.number_of_coders;
    j = 0;
    while (j < n)
    {
        pthread_mutex_lock(&sim->dongles[j].mutex);
        pthread_cond_broadcast(&sim->dongles[j].cond);
        pthread_mutex_unlock(&sim->dongles[j].mutex);
        j++;
    }
}



static int all_finished(t_sim *sim)
{
    int i;
    int n;
    int req;
    int cnt;

    n = sim->config.number_of_coders;
    req = sim->config.number_of_compiles_required;
    if (req <= 0)
        return (0);

    i = 0;
    while (i < n)
    {
        pthread_mutex_lock(&sim->coders[i].data_mutex);
        cnt = sim->coders[i].compile_count;
        pthread_mutex_unlock(&sim->coders[i].data_mutex);

        if (cnt < req)
            return (0);
        i++;
    }
    return (1);
}

void *monitor_routine(void *arg)
{
    t_sim   *sim = (t_sim *)arg;
    int     i;
    int     n = sim->config.number_of_coders;
    long    last;
    long    now;
    long    deadline;
    int     compiling;

    while (!sim_should_stop(sim))
    {
        now = now_ms();

        // 1) Burnout scan
        i = 0;
        while (i < n)
        {
            pthread_mutex_lock(&sim->coders[i].data_mutex);
            last = sim->coders[i].last_compile_start;
            compiling = sim->coders[i].is_compiling;
            pthread_mutex_unlock(&sim->coders[i].data_mutex);

            deadline = last + sim->config.time_to_burnout;

            // You can even remove compiling check if you want stricter rule
            if (compiling == 0 && now >= deadline)
            {
                log_burnout(sim, sim->coders[i].id);
                sim_set_stop(sim, 1);
                wake_all_dongles(sim);
                return (NULL);
            }
            i++;
        }

        // 2) Finished check ONCE per cycle
        if (all_finished(sim))
        {
            sim_set_stop(sim, 1);
            wake_all_dongles(sim);
            return (NULL);
        }

        usleep(10000);
    }
    return (NULL);
}
