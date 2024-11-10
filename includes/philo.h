/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:20:53 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 04:32:33 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

enum status
{
    STARTING,
    TAKINGFORK,
    EATING,
    SLEEPING,
    THINKING,
    FULL,
    DIED
};

typedef struct s_param
{
    size_t number_of_philosophers;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    int number_of_meals;
    int threads_ready;
    long start_time;
    bool launch;
    bool is_dead;
    bool philos_are_full;
    size_t number_of_full_philos;
    pthread_mutex_t full_lock;
    pthread_mutex_t is_dead_lock;
    pthread_mutex_t launch_lock;
    pthread_mutex_t threads_ready_lock;
    pthread_mutex_t write_lock;
} t_param;

typedef struct s_fork
{
    int id;
    pthread_mutex_t lock;
    int is_availble;
} t_fork;

typedef struct s_philosopher
{
    int id;
    pthread_t thread;
    t_fork *left_fork;
    t_fork *right_fork;
    bool full;
    bool last_msg;
    int number_of_meals;
    unsigned long long last_meal_time;
    enum status status;
    pthread_mutex_t status_lock;
    pthread_mutex_t lock;
    pthread_mutex_t meals_lock;
    t_param *param;
} t_philosopher;

typedef struct s_monitor
{
    pthread_t thread;
    t_param *param;
    t_philosopher *philos;
} t_monitor;


// LIBFT UTILS
int	ft_isdigit(int arg);
int	ft_atoi(const char *str);
int ft_strcmp(char *s1, char *s2);

// PARSING
int get_data(int ac, char **av, t_param *param);

// INIT DATA
int __init(t_philosopher **philos, t_fork **forks, t_param *param,  t_monitor **monitor);

// THREADS STUFF
int start_sumulation(t_philosopher *philos, t_monitor *monitor);
int threads_join(t_philosopher *philos, t_monitor *monitor);
void    *monitor_routine(void *data);
int handle_one_philo(t_philosopher *philo);
void    *routine (void *data);

// UTILS
long get_time(void);
void change_status(t_philosopher *philo, enum status status);
void	ft_usleep(long time_to_sleep, t_philosopher *philo);
enum status get_status(t_philosopher *philo);
void write_state(char *s, t_philosopher *philo, bool stop);

// DEATH
bool is_philo_dead(t_philosopher *philo);
bool is_dead(t_philosopher *philo);


#endif
