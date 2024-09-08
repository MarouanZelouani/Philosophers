#include "../includes/philo_bonus.h"

int sem_creat(){}

int init_data(t_philosopher **philos, t_param *param)
{
    size_t i;

    i = 0;
    // printf()
    // (void)forks;
    *philos = malloc (sizeof(t_philosopher) * param->number_of_philosophers);
    if (*philos == NULL)
        return (EXIT_FAILURE);
    while (i < param->number_of_philosophers)
    {
        (*philos)[i].id = i + 1;
        (*philos)[i].number_of_meals = 0;
        (*philos)[i].full = false;
        (*philos)[i].last_msg = false;
        //(*philos)[i].status = STARTING;
        // ASSIGN FORKS
        //printf("i = %ld | %ld\n", i, param->number_of_philosophers);
        //exit(1);
        // (*philos)[i].right_fork = &forks[i];
        // (*philos)[i].left_fork = &forks[(i + 1) % param->number_of_philosophers];
        (*philos)[i].param = param;
        i++;
    }    
    return (EXIT_SUCCESS);    
}