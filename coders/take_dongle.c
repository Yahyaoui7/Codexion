
#include "codexion.h"


int take_dongle(t_sim *sim, t_dongle *d)
{
    long now;

    unsigned long my_ticket = d->next_ticket;
    d->next_ticket++;
    pthread_mutex_lock(&d->mutex);
    while (1)
    {
        if (sim_should_stop(sim))
        {
            pthread_mutex_unlock(&d->mutex);
            return (0);
        }
        now = now_ms();
        if ( my_ticket == d->serving_ticket && d->available && now >= d->cooldown_until)
            break;
        pthread_cond_wait(&d->cond, &d->mutex);
    }
    d->available = 0;
    pthread_mutex_unlock(&d->mutex);
    return (1);
}

void release_dongle(t_sim *sim, t_dongle *d)
{
    pthread_mutex_lock(&d->mutex);

    d->available = 1;
    d->cooldown_until = now_ms() + sim->config.dongle_cooldown;
    d->serving_ticket++;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}


int take_two_dongles(t_coder *c)
{
    t_dongle *a;
    t_dongle *b;


    a = c->left;
    b = c->right;

    if (a > b)
    {
        t_dongle *tmp = a;
        a = b;
        b = tmp;
    }

    if (!take_dongle(c->sim, a))
        return (0);
    log_state(c->sim, c->id, "has taken a dongle");

    if (!take_dongle(c->sim, b))
    {
        release_dongle(c->sim, a);
        return (0);
    }
    log_state(c->sim, c->id, "has taken a dongle");

    return (1);
}
