#include "../includes/philo_bonus.h"


int is_dead(t_philosopher *p)
{
    int death;

    death = 0;
    sem_wait(p->param->death_sem);
    if (get_time() - p->last_meal_time > p->param->time_to_die)
    {
        write_state("died", p);
        death = 1;
    }
    sem_post(p->param->death_sem);
    return (death);
}

int philo_routine(t_philosopher *p)
{
    int s_end;

    s_end = 0;
    while (1)
    {   
        // EATING
        if (is_dead(p))
            exit(1);
        sem_wait(p->param->forks_sem);
        sem_wait(p->param->forks_sem);
        if (is_dead(p))
            exit(1);
        // last_meal_time
        p->last_meal_time = get_time();
        write_state("is eating", p);
        ft_usleep(p->param->time_to_eat);
        if (is_dead(p))
        {
            sem_wait(p->param->forks_sem); 
            sem_post(p->param->forks_sem);
            exit(1);
        }
        // number_of_meals
        p->number_of_meals++;        
        
        // RELEASE FORKS
        sem_wait(p->param->forks_sem); 
        if (is_dead(p))
            exit(1);
        sem_post(p->param->forks_sem);
        //  SLEEPING
        write_state("is sleeping", p);
        ft_usleep(p->param->time_to_sleep);
        if (is_dead(p))
            exit(1);
        // THINKING
        write_state("is thinking", p);
        
        // check number of meals
        if (p->number_of_meals >= p->param->number_of_meals 
            && p->param->number_of_meals != -1)
            break;
    }
    exit(EXIT_SUCCESS);
}

void wait_philos(t_philosopher *philos)
{
    
}

int start_simulation(t_philosopher *philos, t_param *param)
{
    size_t i;

    i = 0;
    // START
    param->start_time = get_time();
    while (i < param->number_of_philosophers)
    {
        philos[i].p_id = fork();
        if (philos[i].p_id == 0)
            philo_routine(&philos[i]);
        else if (philos[i].p_id < 0)
            return (EXIT_FAILURE);
        i++;
    }
    wait_philos(philos);
    return (EXIT_SUCCESS);
}

