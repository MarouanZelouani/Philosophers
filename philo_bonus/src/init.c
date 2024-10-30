#include "../includes/philo_bonus.h"

int sem_creat(t_param *param)
{
    // unlink semaphores
    if (sem_unlink(SEM_FORKS) == -1 || sem_unlink(SEM_WRITE) == -1)
        return (EXIT_FAILURE);
    // open semaphores
    param->forks_sem = sem_open(SEM_FORKS, O_CREAT, 0644, param->number_of_philosophers);
    param->write_sem = sem_open(SEM_WRITE, O_CREAT, 0644, 1);
    if (param->forks_sem != SEM_FAILED || param->write_sem != SEM_FAILED)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int init_data(t_philosopher **philos, t_param *param)
{
    size_t i;

    i = 0;
    *philos = malloc (sizeof(t_philosopher) * param->number_of_philosophers);
    if (*philos == NULL)
        return (EXIT_FAILURE);
    while (i < param->number_of_philosophers)
    {
        (*philos)[i].id = i + 1;
        (*philos)[i].full = false;
        (*philos)[i].last_msg = false;
        (*philos)[i].param = param;
        i++;
    }
    // param->philos_id = malloc (sizeof(pid_t) * param->number_of_philosophers);
    // if (param->philos_id == NULL)
    //     return EXIT_FAILURE;
    return (EXIT_SUCCESS);    
}