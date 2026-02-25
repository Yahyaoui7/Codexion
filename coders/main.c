#include  "codexion.h"

int init_all(t_sim *sim, t_data *cfg)
{
    if (!init_sim(sim, cfg))
        return 0;

    if (!init_dongles(sim))
    {
        destroy_sim(sim);
        return 0;
    }

    if (!init_coders(sim))
    {
        destroy_sim(sim);
        return 0;
    }

    return 1;
}


int main(int ac, char **av)
{
    t_data      data;
    t_sim       sim;
    pthread_t   mon;
    int         i;
    int         n;

    if (ac != 9)
        return (printf("Error: wrong number of arguments\n"), 1);

    memset(&data, 0, sizeof(data));
    parser(av, &data);

    if (!init_all(&sim, &data))
        return (printf("Error: init failed\n"), 1);

    n = sim.config.number_of_coders;
    i = 0;
    while (i < n)
    {
        if (pthread_create(&sim.coders[i].thread, NULL, coder_routine, &sim.coders[i]) != 0)
            break;
        i++;
    }

    if (i != n)
    {
        sim_set_stop(&sim, 1);
        wake_all_dongles(&sim);
        while (i > 0)
        {
            i--;
            pthread_join(sim.coders[i].thread, NULL);
        }
        destroy_sim(&sim);
        return (printf("Error: pthread_create failed\n"), 1);
    }

    if (pthread_create(&mon, NULL, monitor_routine, &sim) != 0)
    {
        sim_set_stop(&sim, 1);
        wake_all_dongles(&sim);
        i = 0;
        while (i < n)
        {
            pthread_join(sim.coders[i].thread, NULL);
            i++;
        }
        destroy_sim(&sim);
        return (printf("Error: monitor pthread_create failed\n"), 1);
    }

    pthread_join(mon, NULL);

    i = 0;
    while (i < n)
    {
        pthread_join(sim.coders[i].thread, NULL);
        i++;
    }

    destroy_sim(&sim);
    return (0);
}

