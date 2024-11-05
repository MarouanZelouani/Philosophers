#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>

# define SEM_WRITE "semwrite"
# define SEM_FORKS "semforks" 

# define DEATH_EX_STAUS 1
# define FULL_EX_STATUS 0

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

    sem_t *write_sem;
    sem_t *forks_sem;
} t_param;

typedef struct s_philo
{
    int id;
    pid_t p_id;
    bool full;
    bool last_msg;
    bool is_dead;
    unsigned long long last_meal_time;
    t_param *param;
} t_philosopher;

int get_args(int ac, char **av, t_param **param);
int init_data(t_philosopher **philos, t_param *param);
int start_simulation(t_philosopher *philo, t_param *param);

// UTILS
long get_time(void);
void	ft_usleep(long time_to_sleep);
void write_state(char *s, t_philosopher *philo);
int	ft_atoi(const char *str);
int	ft_isdigit(int arg);
int ft_strcmp(char *s1, char *s2);
void cleanup(t_param *param, t_philosopher *philos);

#endif