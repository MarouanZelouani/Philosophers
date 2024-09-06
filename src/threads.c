#include "../includes/philo.h"

int start_sumulation(t_philosopher *philos, t_supervisor *supervisor)
{
    size_t i;

    i = 0;
    //exit(1);
    //  ARRAY DECLARATION LMOUCHKIL KAIN FI INIT
    //printf("PHILO = %d\n", philos[i].id);
    while (i < philos->param->number_of_philosophers)
    {
        if (pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]) == -1)
            return (EXIT_FAILURE);
        i++;
    }
    if (pthread_create(&supervisor->thread, NULL, &check_for_death, supervisor) == -1)
        return (EXIT_FAILURE);
    // printf("start\n");
    return (EXIT_SUCCESS);
}

int threads_join(t_philosopher *philos, t_supervisor *supervisor)
{
    size_t i;
    size_t philo_number;

    i = 0;
    philo_number = philos[i].param->number_of_philosophers;
    while (i < philo_number)
    {
        pthread_join(philos[i].thread, NULL);
           
        i++;
    }
    pthread_join(supervisor->thread, NULL);
        
    return (EXIT_SUCCESS);
}

