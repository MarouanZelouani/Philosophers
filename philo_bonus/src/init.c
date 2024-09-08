#include "../includes/philo_bonus.h"

int sem_creat(t_param *param)
{
    param->death_sem = sem_open(SEM_DEATH, O_CREAT, 0644, 1);
    param->forks_sem = sem_open(SEM_FORKS, O_CREAT, 0644, 1);
    param->write_sem = sem_open(SEM_WRITE, O_CREAT, 0644, 1);
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
        (*philos)[i].number_of_meals = 0;
        (*philos)[i].full = false;
        (*philos)[i].last_msg = false;
        (*philos)[i].param = param;
        i++;
    } 
    return (EXIT_SUCCESS);    
}