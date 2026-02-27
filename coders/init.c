/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:37:55 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 00:39:46 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	now_ms(void)
{
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	return ((tm.tv_sec * 1000L) + (tm.tv_usec / 1000L));
}

int	init_sim(t_sim *sim, t_data *cfg)
{
	memset(sim, 0, sizeof(*sim));
	sim->start_time = now_ms();
	sim->stop = 0;
	sim->config = *cfg;
	if (pthread_mutex_init(&sim->stop_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&sim->log_mutex, NULL))
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	sim->coders = NULL;
	sim->dongles = NULL;
	return (1);
}

int	init_dongles(t_sim *sim)
{
	int	n;

	n = sim->config.number_of_coders;
	sim->dongles = malloc(sizeof(t_dongle) * n);
	if (!sim->dongles)
		return (0);
	if (!init_dongles_utils(-1, n, sim))
		return (0);
	return (1);
}

int	init_coders(t_sim *sim)
{
	int	n;

	n = sim->config.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * n);
	if (!sim->coders)
		return (0);
	if (!init_coders_utils(n, sim))
		return (0);
	return (1);
}
