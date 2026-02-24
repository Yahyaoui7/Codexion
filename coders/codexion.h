#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_sim     t_sim;
typedef struct s_dongle  t_dongle;
typedef struct s_coder   t_coder;

typedef struct s_data
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    int scheduler; // 0=fifo, 1=edf
}   t_data;

struct s_dongle
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int             available;
    long            cooldown_until;

};

struct s_coder
{
    int         id;
    int         compile_count;
    long        last_compile_start;
    int         is_compiling ;
    pthread_mutex_t data_mutex;
    pthread_t   thread;
    t_dongle    *left;
    t_dongle    *right;
    t_sim       *sim;
};

struct s_sim
{
    long            start_time;
    int             stop;
    t_data          config;
    pthread_mutex_t stop_mutex;
    pthread_mutex_t log_mutex;
    t_coder         *coders;
    t_dongle        *dongles;
};

/*             cleanup          */
void    destroy_dongles(t_sim *sim);
void    destroy_sim(t_sim *sim);

/*       coder      */
void *coder_routine(void *arg);


/*            init       */
int init_sim(t_sim *sim, t_data *cfg);
int init_dongles(t_sim *sim);
int init_coders(t_sim *sim);
long now_ms(void);

/*        logging */
int     sim_should_stop(t_sim *sim);
void    sim_set_stop(t_sim *sim, int v);
void    log_state(t_sim *sim, int id, const char *msg);
void    log_burnout(t_sim *sim, int id);

/*     moniter      */
void *monitor_routine(void *arg);

/*            parser        */
void    parser(char **av, t_data *data);

 /*     take_dongle       */
int take_two_dongles(t_coder *c);

#endif
