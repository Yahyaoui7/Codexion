/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 22:54:58 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/26 23:02:21 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_two_dongles(t_coder *c)
{
	if (c->left)
		release_dongle(c->sim, c->left);
	if (c->right && c->right != c->left)
		release_dongle(c->sim, c->right);
}

static void	compiling(t_coder *c)
{
	pthread_mutex_lock(&c->data_mutex);
	c->is_compiling = 1;
	c->last_compile_start = now_ms();
	c->compile_count++;
	pthread_mutex_unlock(&c->data_mutex);
	log_state(c->sim, c->id, "is compiling");
	usleep(c->sim->config.time_to_compile * 1000);
	pthread_mutex_lock(&c->data_mutex);
	c->is_compiling = 0;
	pthread_mutex_unlock(&c->data_mutex);
}

static void	debugging(t_coder *c)
{
	log_state(c->sim, c->id, "is debugging");
	usleep(c->sim->config.time_to_debug * 1000);
}

static void	refactoring(t_coder *c)
{
	log_state(c->sim, c->id, "is refactoring");
	usleep(c->sim->config.time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	while (!sim_should_stop(c->sim))
	{
		if (!take_two_dongles(c))
			break ;
		compiling(c);
		release_two_dongles(c);
		if (sim_should_stop(c->sim))
			break ;
		debugging(c);
		if (sim_should_stop(c->sim))
			break ;
		refactoring(c);
	}
	return (NULL);
}
