#include "../includes/philo.h"

int start_sumulation(t_philosopher *philos, t_supervisor supervisor)
{
    int i;

    i = 0;
    exit(1);
    //  ARRAY DECLARATION LMOUCHKIL KAIN FI INIT
    printf("PHILO = %d\n", philos[i].id);
    if (pthread_create(&supervisor.thread, NULL, &check_for_death, &supervisor) == -1)
        return (EXIT_FAILURE);
    while (i < NUMBER_OF_PHILOS)
    {
        if (pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]) == -1)
            return (EXIT_FAILURE);
        i++;
    }
    printf("start\n");
    return (EXIT_SUCCESS);
}

int threads_join(t_philosopher *philos, t_supervisor supervisor)
{
    int i;

    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        if (pthread_join(philos[i].thread, NULL) == -1)
            return (EXIT_FAILURE);
        i++;
    }
    if (pthread_join(supervisor.thread, NULL) == -1)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

