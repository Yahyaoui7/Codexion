#include "codexion.h"

static long now_ms()
{
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    return ((tm.tv_sec * 1000L) + (tm.tv_usec / 1000L));
}

int init_sim(t_sim *sim, t_data *cfg)
{
    memset(sim, 0, sizeof(* sim));

    sim->start_time = now_ms();
    sim->stop = 0;
    sim->config = *cfg;

    if (pthread_mutex_init(&sim->stop_mutex, NULL))
        return 0;
    if (pthread_mutex_init(&sim->log_mutex, NULL))
    {
        pthread_mutex_destroy(&sim->stop_mutex);
        return 0;
    }

    sim->coders = NULL;
    sim->dongles = NULL;
    return 1;
}

int init_dongles(t_sim *sim)
{
    int i;
    int n;

    n = sim->config.number_of_coders;

    sim->dongles = malloc(sizeof(t_dongle) * n);
    if (!sim->dongles)
        return 0;

    i = 0;
    while (i < n)
    {
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
            break;

        if (pthread_cond_init(&sim->dongles[i].cond, NULL))
        {
            pthread_mutex_destroy(&sim->dongles[i].mutex);
            break;
        }

        sim->dongles[i].available = 1;
        sim->dongles[i].cooldown_until = 0;
        i++;
    }

    if (i != n)
    {
        while (--i >= 0)
        {
            pthread_mutex_destroy(&sim->dongles[i].mutex);
            pthread_cond_destroy(&sim->dongles[i].cond);
        }
        free(sim->dongles);
        sim->dongles = NULL;
        return 0;
    }

    return 1;
}

int init_coders(t_sim *sim)
{
    int i;
    int n;

    n = sim->config.number_of_coders;
    sim->coders = malloc(sizeof(t_coder) * n);
    if (!sim->coders)
        return 0;

    i = 0;
    while (i < n)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile_start = sim->start_time;

        sim->coders[i].left = &sim->dongles[i];
        sim->coders[i].right = &sim->dongles[(i + 1) % n];

        sim->coders[i].sim = sim;
        i++;
    }
    return 1;
}
