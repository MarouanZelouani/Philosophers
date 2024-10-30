#include "../includes/philo_bonus.h"

int is_dead(t_philosopher *p)
{
    if (get_time() - p->last_meal_time > p->param->time_to_die)
    {
        write_state("died", p);
        return (1);
    }
    return (0);
}

int philo_routine(t_philosopher *p)
{
    int s_end;
    int number_of_meals;

    s_end = 0;
    number_of_meals = 0;
    while (1)
    {   
        if (is_dead(p))
            exit(DEATH_EX_STAUS);
        sem_wait(p->param->forks_sem);
        sem_wait(p->param->forks_sem);
        if (is_dead(p))
            exit(DEATH_EX_STAUS);
        p->last_meal_time = get_time();
        write_state("is eating", p);
        ft_usleep(p->param->time_to_eat);
        if (is_dead(p))
        {
            sem_wait(p->param->forks_sem); 
            sem_post(p->param->forks_sem);
            exit(DEATH_EX_STAUS);
        }
        number_of_meals++;        
        sem_wait(p->param->forks_sem); 
        if (is_dead(p))
            exit(DEATH_EX_STAUS);
        sem_post(p->param->forks_sem);
        write_state("is sleeping", p);
        ft_usleep(p->param->time_to_sleep);
        if (is_dead(p))
            exit(DEATH_EX_STAUS);
        write_state("is thinking", p);        
        if (number_of_meals >= p->param->number_of_meals 
            && p->param->number_of_meals != -1)
            break;
    }
    exit(FULL_EX_STATUS);
}

void wait_philos(t_philosopher *philos, t_param *param)
{
    size_t i;

    i = 0;
    while (i < param->number_of_philosophers)
    {
        waitpid(philos[i].p_id, NULL, 0);
        i++;
    }
}

void monitor(t_philosopher *philos, t_param *param)
{
    pid_t exited_pid;
    size_t i;
    int status;

    i = 0;
    while (1)
    {
        exited_pid = waitpid(-1, &status, 0);
        if (exited_pid > 0)
        {
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                // Kill all other philosophers
                while (i < param->number_of_philosophers)
                {
                    if (philos[i].p_id != exited_pid)
                        kill(philos[i].p_id, SIGKILL);
                    i++;
                }
                break;
            }
        }
        else
            break;
    }
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
    monitor(philos, param);
    wait_philos(philos, param);
    return (EXIT_SUCCESS);
}

