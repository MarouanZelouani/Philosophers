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
#include <fcntl.h>

# define SEM_DEATH "/semdeath"
# define SEM_WRITE "/semwrite"
# define SEM_FORKS "/semforks" 

typedef struct s_param
{
    size_t number_of_philosophers;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    ssize_t number_of_meals;

    int threads_ready;
    long start_time;
    bool launch;
    int death;
    bool philos_are_full;
    size_t number_of_full_philos;

    pid_t *philos_id; // remove this

    sem_t *death_sem;
    sem_t *write_sem;
    sem_t *forks_sem;
} t_param;

typedef struct s_philo
{
    int id;
    pid_t p_id;
    bool full;
    bool last_msg;
    int number_of_meals;
    unsigned long long last_meal_time;
    t_param *param;
} t_philosopher;

int get_args(int ac, char **av, t_param *param);
int init_data(t_philosopher **philos, t_param *param);
int start_simulation(t_philosopher *philo, t_param *param);

// UTILS
long get_time(void);
void	ft_usleep(long time_to_sleep);
void write_state(char *s, t_philosopher *philo);

#endif