/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 13:50:35 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 00:37:21 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_destroy(t_heap *q)
{
	free(q->arr);
	q->arr = NULL;
	q->size = 0;
	q->capacity = 0;
}

void	destroy_dongles(t_sim *sim)
{
	int	i;
	int	n;

	if (!sim->dongles)
		return ;
	n = sim->config.number_of_coders;
	i = 0;
	while (i < n)
	{
		heap_destroy(&sim->dongles[i].queue);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
}

void	destroy_sim(t_sim *sim)
{
	if (!sim)
		return ;
	destroy_dongles(sim);
	free(sim->dongles);
	sim->dongles = NULL;
	free(sim->coders);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
}
