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

static int	take_if_my_turn(t_request top, t_dongle *d, int coder_id)
{
	if (top.coder_id == coder_id && d->available)
	{
		heap_pop(&d->queue, &top);
		d->available = 0;
		pthread_mutex_unlock(&d->mutex);
		return (1);
	}
	return (0);
}

static int	try_take_dangle(t_sim *sim, t_dongle *d, int coder_id)
{
	t_request	top;
	long		now;

	while (1)
	{
		if (sim_should_stop(sim))
		{
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
		if (!heap_peek(&d->queue, &top))
		{
			pthread_cond_wait(&d->cond, &d->mutex);
			continue ;
		}
		now = now_ms();
		if (now < d->cooldown_until)
		{
			wait_until_ms(&d->cond, &d->mutex, d->cooldown_until);
			continue ;
		}
		if (take_if_my_turn(top, d, coder_id))
			return (1);
		pthread_cond_wait(&d->cond, &d->mutex);
	}
}

int	take_dongle(t_sim *sim, t_dongle *d, int coder_id, long priority)
{
	t_request	my_req;

	pthread_mutex_lock(&d->mutex);
	my_req.coder_id = coder_id;
	my_req.seq = d->ticket_counter++;
	if (sim->config.scheduler == 0)
		my_req.priority = my_req.seq;
	else
		my_req.priority = priority;
	if (!heap_push(&d->queue, my_req))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	if (!try_take_dangle(sim, d, coder_id))
		return (0);
	return (1);
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
	pa = get_req_priority(c);
	if (!take_dongle(c->sim, a, c->id, pa))
		return (0);
	pb = get_req_priority(c);
	if (!take_dongle(c->sim, b, c->id, pb))
	{
		release_dongle(c->sim, a);
		return (0);
	}
	log_state(c->sim, c->id, "has taken a dongle");
	log_state(c->sim, c->id, "has taken a dongle");
	return (1);
}
