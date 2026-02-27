/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 13:32:32 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 00:39:25 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_should_stop(t_sim *sim)
{
	int	v;

	pthread_mutex_lock(&sim->stop_mutex);
	v = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (v);
}

void	sim_set_stop(t_sim *sim, int v)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = v;
	pthread_mutex_unlock(&sim->stop_mutex);
}

void	log_state(t_sim *sim, int id, const char *msg)
{
	long	ts;

	if (sim_should_stop(sim))
		return ;
	pthread_mutex_lock(&sim->log_mutex);
	ts = now_ms() - sim->start_time;
	printf("%ld %d %s\n", ts, id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	log_burnout(t_sim *sim, int id)
{
	long	ts;

	ts = now_ms() - sim->start_time;
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d burned out\n", ts, id);
	pthread_mutex_unlock(&sim->log_mutex);
}
