/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:38:52 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/27 00:38:54 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_sim		t_sim;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_request	t_request;
typedef struct s_heap		t_heap;

struct						s_request
{
	int						coder_id;
	long					priority;
	long					seq;
};

struct						s_heap
{
	t_request				*arr;
	int						size;
	int						capacity;
};

typedef struct s_data
{
	int						number_of_coders;
	int						time_to_burnout;
	int						time_to_compile;
	int						time_to_debug;
	int						time_to_refactor;
	int						number_of_compiles_required;
	int						dongle_cooldown;
	int						scheduler;
}							t_data;

struct						s_dongle
{
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	int						available;
	long					cooldown_until;
	long					ticket_counter;
	t_heap					queue;
};

struct						s_coder
{
	int						id;
	int						compile_count;
	long					last_compile_start;
	int						is_compiling;
	pthread_mutex_t			data_mutex;
	pthread_t				thread;
	t_dongle				*left;
	t_dongle				*right;
	t_sim					*sim;
};

struct						s_sim
{
	long					start_time;
	int						stop;
	t_data					config;
	pthread_mutex_t			stop_mutex;
	pthread_mutex_t			log_mutex;
	t_coder					*coders;
	t_dongle				*dongles;
};

/*             cleanup          */
void						destroy_dongles(t_sim *sim);
void						destroy_sim(t_sim *sim);
void						heap_destroy(t_heap *q);

/*       coder      */
void						*coder_routine(void *arg);

/*            init       */
int							init_sim(t_sim *sim, t_data *cfg);
int							init_dongles(t_sim *sim);
int							init_coders(t_sim *sim);
long						now_ms(void);

/*            init_utils       */
int							init_dongles_utils(int n, t_sim *sim);
int							init_coders_utils(int n, t_sim *sim);
int							heap_init(t_heap *q, int n);

/*        logging               */
int							sim_should_stop(t_sim *sim);
void						sim_set_stop(t_sim *sim, int v);
void						log_state(t_sim *sim, int id, const char *msg);
void						log_burnout(t_sim *sim, int id);

/*     moniter      */
void						*monitor_routine(void *arg);
void						wake_all_dongles(t_sim *sim);

/*            parser        */
void						parser(char **av, t_data *data);

/*     take_dongle       */
int							take_two_dongles(t_coder *c);
void						release_dongle(t_sim *sim, t_dongle *d);

/*       manger*/
void						manger(t_sim *sim);

/*              heap                 */
int							heap_push(t_heap *q, t_request r);
int							less_req(t_request a, t_request b);
void						swap_req(t_request *a, t_request *b);

/*          heao_utils        */
int							heap_pop(t_heap *q, t_request *out);
int							heap_peek(t_heap *q, t_request *out);

/*            take_dongel_utils     */
void						wait_until_ms(pthread_cond_t *cond,
								pthread_mutex_t *mtx, long abs_ms);
long						get_req_priority(t_coder *c);
void						release_dongle(t_sim *sim, t_dongle *d);

#endif
