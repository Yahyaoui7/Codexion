/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongls_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:53:48 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/03/03 13:54:03 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_until_ms(pthread_cond_t *cond, pthread_mutex_t *mtx, long abs_ms)
{
	struct timespec	ts;

	ts.tv_sec = abs_ms / 1000L;
	ts.tv_nsec = (abs_ms % 1000L) * 1000000L;
	pthread_cond_timedwait(cond, mtx, &ts);
}

long	get_req_priority(t_coder *c)
{
	if (c->sim->config.scheduler == 1)
		return (c->last_compile_start + c->sim->config.time_to_burnout);
	return (0);
}

void	release_dongle(t_sim *sim, t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->available = 1;
	d->cooldown_until = now_ms() + sim->config.dongle_cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
