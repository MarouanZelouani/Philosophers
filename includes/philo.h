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

#define NUMBER_OF_PHILOS 5
#define TIME_TO_DIE 210
#define TIME_TO_EAT 200
#define TIME_TO_SLEEP 100
#define MAX_MEALS -1

enum status 
{
    STARTING,
    TAKINGFORK,
    EATING,
    SLEEPING,
    THINKING,
    DIED
};

typedef struct s_param
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_meals;
    int threads_ready;
    long start_time;
    bool launch;
    bool is_dead;
    bool philos_are_full;
    int number_of_full_philos;
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
    long last_meal_time;
    enum status status;
    pthread_mutex_t status_lock;
    pthread_mutex_t lock;
    pthread_mutex_t meals_lock;
    t_param *param;
} t_philosopher;

typedef struct s_supervisor
{
    pthread_t thread;
    t_param *param;
    t_philosopher *philos;
} t_supervisor;

#endif
