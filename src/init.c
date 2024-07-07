#include "../includes/philo.h"

// INIT DATA
int param_init(t_param *param)
{
        // ALLOCATION
    param->threads_ready = 0;
    param->launch = false;
    param->is_dead = false;
    param->start_time = get_time(); // START     
    // ALLWAYS INIT YOUR MUTEXS!!
    if (pthread_mutex_init(&param->write_lock, NULL) == -1)
        return (EXIT_FAILURE);
    if (pthread_mutex_init(&param->threads_ready_lock, NULL) == -1)
        return (EXIT_FAILURE);
    if (pthread_mutex_init(&param->launch_lock, NULL) == -1)
        return (EXIT_FAILURE);
    if (pthread_mutex_init(&param->is_dead_lock, NULL) == -1);
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

// INIT FORKS
int forks_init(t_fork *forks, t_param *param)
{
    int i;
    int error;

    i = 0;
    error = 0;
    forks = malloc (sizeof(t_fork) * param->number_of_philosophers);
    if (forks == NULL)
        return (EXIT_FAILURE);
    while (i < param->number_of_philosophers)
    {
        if (pthread_mutex_init(&(forks[i].lock), NULL));
            return (EXIT_FAILURE);
        forks[i].id = i;
        forks[i].is_availble = 1;
        i++;
    }
    return (EXIT_SUCCESS);
}


// INIT PHILOSOPHERS
int philosophers_init(t_philosopher *philos, t_fork *forks, t_param *param)
{
    int i;

    i = 0;
    philos = malloc (sizeof(t_philosopher) * param->number_of_philosophers);
    if (philos == NULL)
        return (EXIT_FAILURE);
    while (i < param->number_of_philosophers)
    {
        philos[i].id = i + 1;
        philos[i].number_of_meals = 0;
        philos[i].full = false;
        philos[i].last_msg = false;
        philos[i].status = STARTING;
        // ASSIGN FORKS
        philos[i].right_fork = &forks[i];
        philos[i].left_fork = &forks[(i + 1) % NUMBER_OF_PHILOS];
        philos[i].param = param;
        if (pthread_mutex_init(&philos[i].status_lock, NULL) == -1)
            return (EXIT_FAILURE);
        if (pthread_mutex_init(&philos[i].lock, NULL) == -1)
            return (EXIT_FAILURE);
        if (pthread_mutex_init(&philos[i].meals_lock, NULL) == -1)
            return (EXIT_FAILURE);
        i++;
    }    
    return (EXIT_SUCCESS);
}

int data_init(t_param *param, t_philosopher *philos, t_fork *forks, t_supervisor *supervisor)
{   
    // INIT DATA
    if (!param_init(param))
        return (EXIT_FAILURE);
    else if (!philosophers_init(philos, forks, param))
        return (EXIT_FAILURE);
    else if (!forks_init(forks, param))
        return (EXIT_FAILURE);
    supervisor->param = param;
    supervisor->philos = philos;
    return (EXIT_SUCCESS);
}