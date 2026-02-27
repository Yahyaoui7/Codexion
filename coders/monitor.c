/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 22:42:53 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 00:39:39 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static int	all_finished(t_sim *sim)
{
	int	i;
	int	n;
	int	req;
	int	cnt;

	n = sim->config.number_of_coders;
	req = sim->config.number_of_compiles_required;
	i = 0;
	while (i < n)
	{
		pthread_mutex_lock(&sim->coders[i].data_mutex);
		cnt = sim->coders[i].compile_count;
		pthread_mutex_unlock(&sim->coders[i].data_mutex);
		if (cnt < req)
			return (0);
		i++;
	}
	return (1);
}

int	check_burnout(t_sim *sim)
{
	int		i;
	long	last;
	int		n;

	i = 0;
	n = sim->config.number_of_coders;
	while (i < n)
	{
		pthread_mutex_lock(&sim->coders[i].data_mutex);
		last = sim->coders[i].last_compile_start;
		pthread_mutex_unlock(&sim->coders[i].data_mutex);
		if (now_ms() - last >= sim->config.time_to_burnout)
		{
			log_burnout(sim, sim->coders[i].id);
			sim_set_stop(sim, 1);
			wake_all_dongles(sim);
			return (0);
		}
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!sim_should_stop(sim))
	{
		if (!check_burnout(sim))
			return (NULL);
		if (all_finished(sim))
		{
			sim_set_stop(sim, 1);
			wake_all_dongles(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
