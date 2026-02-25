#include "codexion.h"

// void *coder_routine(void *arg)
// {
//     t_coder *c = (t_coder *)arg;

//     while (!sim_should_stop(c->sim))
//     {
//         if (!take_two_dongles(c))
//             break;

//         pthread_mutex_lock(&c->data_mutex);
//         c->is_compiling = 1;
//         c->last_compile_start = now_ms();
//         pthread_mutex_unlock(&c->data_mutex);

//         log_state(c->sim, c->id, "is compiling");
//         usleep(c->sim->config.time_to_compile * 1000);

//         pthread_mutex_lock(&c->data_mutex);
//         c->is_compiling = 0;
//         c->compile_count++;
//         pthread_mutex_unlock(&c->data_mutex);

//         release_dongle(c->sim, c->left);
//         release_dongle(c->sim, c->right);

//         log_state(c->sim, c->id, "is debugging");
//         usleep(c->sim->config.time_to_debug * 1000);

//         log_state(c->sim, c->id, "is refactoring");
//         usleep(c->sim->config.time_to_refactor * 1000);
//     }
//     return NULL;
// }

void release_two_dongles(t_coder *c)
{
    if (c->left)
        release_dongle(c->sim, c->left);
    if (c->right && c->right != c->left)
        release_dongle(c->sim, c->right);
}

void *coder_routine(void *arg)
{
    t_coder *c = (t_coder *)arg;

    while (!sim_should_stop(c->sim))
    {
        if (!take_two_dongles(c))
        {
            usleep(500);
            continue;
        }



        pthread_mutex_lock(&c->data_mutex);
        c->is_compiling = 1;
        c->last_compile_start = now_ms();
        c->compile_count++;
        pthread_mutex_unlock(&c->data_mutex);


        log_state(c->sim, c->id, "is compiling");
        usleep(c->sim->config.time_to_compile * 1000);


        pthread_mutex_lock(&c->data_mutex);
        c->is_compiling = 0;
        pthread_mutex_unlock(&c->data_mutex);


        release_two_dongles(c);

        if (sim_should_stop(c->sim))
            break;


        log_state(c->sim, c->id, "is debugging");
        usleep(c->sim->config.time_to_debug * 1000);

        if (sim_should_stop(c->sim))
            break;


        log_state(c->sim, c->id, "is refactoring");
        usleep(c->sim->config.time_to_refactor * 1000);
    }
    return NULL;
}
