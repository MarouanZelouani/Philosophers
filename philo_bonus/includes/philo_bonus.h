#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>

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
    sem_t *semaphores;
} t_param;

typedef struct s_philo
{
    int id;
    bool full;
    bool last_msg;
    int number_of_meals;
    unsigned long long last_meal_time;
    t_param *param;
} t_philosopher;

int get_args(int ac, char **av, t_param *param);
int init_data(t_philosopher **philos, t_param *param);

#endif