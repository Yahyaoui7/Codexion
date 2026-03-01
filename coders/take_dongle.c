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

// static void	time_wait_dongle(t_dongle *d)
// {
// 	struct timespec	ts;

// 	ts.tv_sec = d->cooldown_until / 1000;
// 	ts.tv_nsec = (d->cooldown_until % 1000) * 1000000L;
// 	pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
// }

int	take_dongle(t_sim *sim, t_dongle *d, int coder_id, long priority)
{
	t_request	my_req;
	t_request	top;

	pthread_mutex_lock(&d->mutex);
	my_req.coder_id = coder_id;
	my_req.priority = priority;
	if (!heap_push(&d->queue, my_req))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	while (1)
	{
		if (sim_should_stop(sim))
		{
			heap_remove_by_id(&d->queue, coder_id);
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
		if (!heap_peek(&d->queue, &top))
		{
			pthread_cond_wait(&d->cond, &d->mutex);
			continue ;
		}
		if (top.coder_id == coder_id && d->available
			&& now_ms() >= d->cooldown_until)
		{
			heap_pop(&d->queue, &top);
			d->available = 0;
			pthread_mutex_unlock(&d->mutex);
			return (1);
		}
		pthread_cond_wait(&d->cond, &d->mutex);
	}
}

void	release_dongle(t_sim *sim, t_dongle *d)
{
	(void)sim;
	pthread_mutex_lock(&d->mutex);
	d->available = 1;
	d->cooldown_until = now_ms() + 0;
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

static long	compute_priority(t_coder *c, t_dongle *d)
{
	(void)d;
	if (c->sim->config.scheduler == 1)
		return (c->last_compile_start + c->sim->config.time_to_burnout);
	return (0);
}

int	take_two_dongles(t_coder *c)
{
	t_dongle	*a;
	t_dongle	*b;
	long		pa;
	long		pb;

	pthread_mutex_lock(&c->data_mutex);
	if (c->compile_count >= c->sim->config.number_of_compiles_required)
	{
		pthread_mutex_unlock(&c->data_mutex);
		return (0);
	}
	pthread_mutex_unlock(&c->data_mutex);
	select_dongles(&a, &b, c);
	pa = compute_priority(c, a);
	if (!take_dongle(c->sim, a, c->id, pa))
		return (0);
	pb = compute_priority(c, b);
	if (!take_dongle(c->sim, b, c->id, pb))
	{
		release_dongle(c->sim, a);
		return (0);
	}
	log_state(c->sim, c->id, "has taken a dongle");
	log_state(c->sim, c->id, "has taken a dongle");
	return (1);
}
