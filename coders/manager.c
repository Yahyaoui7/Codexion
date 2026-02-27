/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 12:57:23 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/26 14:00:27 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	creat_monetor(t_sim *sim, pthread_t *mon)
{
	int	i;
	int	n;

	i = 0;
	n = sim->config.number_of_coders;
	if (pthread_create(mon, NULL, monitor_routine, sim) != 0)
	{
		sim_set_stop(sim, 1);
		wake_all_dongles(sim);
		i = 0;
		while (i < n)
		{
			pthread_join(sim->coders[i].thread, NULL);
			i++;
		}
		destroy_sim(sim);
		printf("Error: monitor pthread_create failed\n");
		exit(1);
	}
}

static void	creat_coders(t_sim *sim)
{
	int	n;
	int	i;

	n = sim->config.number_of_coders;
	i = 0;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			break ;
		i++;
	}
	if (i != n)
	{
		sim_set_stop(sim, 1);
		wake_all_dongles(sim);
		while (i > 0)
		{
			i--;
			pthread_join(sim->coders[i].thread, NULL);
		}
		destroy_sim(sim);
		printf("Error: pthread_create failed\n");
		exit(1);
	}
}

void	manger(t_sim *sim)
{
	int			n;
	int			i;
	pthread_t	mon;

	creat_coders(sim);
	creat_monetor(sim, &mon);
	n = sim->config.number_of_coders;
	i = 0;
	while (i < n)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(mon, NULL);
}
