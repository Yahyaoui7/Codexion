#include "codexion.h"

void *coder_routine(void *arg)
{
    t_coder *c = (t_coder *)arg;

    while (!sim_should_stop(c->sim))
    {

        pthread_mutex_lock(&c->data_mutex);
        c->is_compiling = 1;
        c->last_compile_start = now_ms();

        pthread_mutex_unlock(&c->data_mutex);
        log_state(c->sim, c->id, "is compiling");
        usleep(c->sim->config.time_to_compile * 1000);

        pthread_mutex_lock(&c->data_mutex);
        c->compile_count++;
        c->is_compiling = 0;
        pthread_mutex_unlock(&c->data_mutex);


        log_state(c->sim, c->id, "is debugging");
        usleep(c->sim->config.time_to_debug * 1000);

        log_state(c->sim, c->id, "is refactoring");
        usleep(c->sim->config.time_to_refactor * 1000);
    }
    return NULL;
}

