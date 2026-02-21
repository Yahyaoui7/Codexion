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
    t_data data;
    t_sim  sim;

    if (ac != 9)
        return (printf("Error: wrong number of arguments\n"), 1);

    memset(&data, 0, sizeof(data));
    parser(av, &data);

    if (!init_all(&sim, &data))
        return (printf("Error: init failed\n"), 1);

    /* later: start threads + monitor here */

    destroy_sim(&sim);
    return 0;
}
