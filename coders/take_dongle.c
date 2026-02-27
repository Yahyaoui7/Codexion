/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 23:08:20 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 00:32:54 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	time_wait_dongle(t_dongle *d)
{
	struct timespec	ts;

	ts.tv_sec = d->cooldown_until / 1000;
	ts.tv_nsec = (d->cooldown_until % 1000) * 1000000L;
	pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
}

int	take_dongle(t_sim *sim, t_dongle *d)
{
	unsigned long	my_ticket;

	pthread_mutex_lock(&d->mutex);
	my_ticket = d->next_ticket++;
	while (1)
	{
		if (sim_should_stop(sim))
		{
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
		if (my_ticket == d->serving_ticket && d->available)
		{
			if (now_ms() >= d->cooldown_until)
			{
				d->available = 0;
				pthread_mutex_unlock(&d->mutex);
				return (1);
			}
			time_wait_dongle(d);
		}
		else
			pthread_cond_wait(&d->cond, &d->mutex);
	}
}

void	release_dongle(t_sim *sim, t_dongle *d)
{
	long	now;

	now = now_ms();
	pthread_mutex_lock(&d->mutex);
	d->available = 1;
	d->cooldown_until = now + sim->config.dongle_cooldown;
	d->serving_ticket++;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}

static void	select_dongles(t_dongle **a, t_dongle **b, t_coder *c)
{
	t_dongle	*tmp;

	*a = c->left;
	*b = c->right;
	if (*a > *b)
	{
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
}

int	take_two_dongles(t_coder *c)
{
	t_dongle	*a;
	t_dongle	*b;

	pthread_mutex_lock(&c->data_mutex);
	if (c->sim->config.number_of_compiles_required > 0
		&& c->compile_count >= c->sim->config.number_of_compiles_required)
	{
		pthread_mutex_unlock(&c->data_mutex);
		return (0);
	}
	pthread_mutex_unlock(&c->data_mutex);
	select_dongles(&a, &b, c);
	if (!take_dongle(c->sim, a))
		return (0);
	if (!take_dongle(c->sim, b))
	{
		release_dongle(c->sim, a);
		return (0);
	}
	log_state(c->sim, c->id, "has taken a dongle");
	log_state(c->sim, c->id, "has taken a dongle");
	return (1);
}
