#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define TIME_TO_EAT 200
#define TIME_TO_SLEEP 210
#define TIME_TO_DIE 200
#define NUMBER_OF_PHILOS 4
#define MAX_MEALS 3


typedef struct s_param
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_meals;
    int threads_ready;
    long start_time;
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
    int number_of_meals;
    long last_meal_time;
    t_param *param;
} t_philosopher;

#endif
