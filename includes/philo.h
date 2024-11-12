/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:20:53 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/11 22:09:32 by mzelouan         ###   ########.fr       */
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

typedef enum s_status
{
    STARTING,
    TAKENFORK,
    EATING,
    SLEEPING,
    THINKING,
    FULL,
    DIED
} t_status;

typedef struct s_args
{
    size_t number_of_philosophers;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    int number_of_meals;
    long start_time;
    bool is_dead;
    pthread_mutex_t is_dead_lock;
    pthread_mutex_t write_lock;
} t_args;

typedef struct s_fork
{
    int id;
    pthread_mutex_t lock;
    int is_availble;
} t_fork;

typedef struct s_philo
{
    int id;
    pthread_t thread;
    t_fork *left_fork;
    t_fork *right_fork;
    t_args *args;
    int number_of_meals;
    unsigned long long last_meal_time;
    t_status status;
    pthread_mutex_t status_lock;
    pthread_mutex_t lock;
    pthread_mutex_t meals_lock;
} t_philo;

typedef struct s_monitor
{
    pthread_t thread;
    t_args *args;
    t_philo *philos;
} t_monitor;


// LIBFT UTILS
int	ft_isdigit(int arg);
int	ft_atoi(const char *str);
int ft_strcmp(char *s1, char *s2);

// PARSING
int get_data(int ac, char **av, t_args *args);

// INIT DATA
int __init(t_philo **philos, t_fork **forks, t_args *args,  t_monitor **monitor);

// THREADS TOOLS
int start_sumulation(t_philo *philos, t_monitor *monitor);
int threads_join(t_philo *philos, t_monitor *monitor);
void    *monitor_routine(void *data);
int handle_one_philo(t_philo *philo);
void    *routine (void *data);

// UTILS
long get_time(void);
void change_status(t_philo *philo, t_status status);
void	gosleep(long time_to_sleep, t_philo *philo);
t_status get_status(t_philo *philo);
void write_state(char *s, t_philo *philo, bool stop);

// DEATH CHECKS
bool is_philo_dead(t_philo *philo);
bool is_dead(t_philo *philo);

// CLEAN/ 
void cleanup(t_philo *philos, t_monitor *monitor, t_args *args, t_fork *forks);
#endif
