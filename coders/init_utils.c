/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:13:08 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 17:13:24 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles_utils(int n, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
			break ;
		if (pthread_cond_init(&sim->dongles[i].cond, NULL))
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			break ;
		}
		sim->dongles[i].available = 1;
		sim->dongles[i].cooldown_until = 0;
		if (!heap_init(&sim->dongles[i].queue, n))
		{
			pthread_cond_destroy(&sim->dongles[i].cond);
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			break ;
		}
		i++;
	}
	return (i);
}

int	init_coders_utils(int n, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % n];
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].data_mutex, NULL) != 0)
			break ;
		sim->coders[i].is_compiling = 0;
		i++;
	}
	if (i != n)
	{
		while (i-- > 0)
			pthread_mutex_destroy(&sim->coders[i].data_mutex);
		free(sim->coders);
		sim->coders = NULL;
		return (0);
	}
	return (1);
}

int	heap_init(t_heap *q, int n)
{
	q->capacity = n;
	q->size = 0;
	q->arr = malloc(sizeof(t_request) * n);
	if (!q->arr)
		return (0);
	return (1);
}
