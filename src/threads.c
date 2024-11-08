#include "../includes/philo.h"

int start_sumulation(t_philosopher *philos, t_monitor *monitor)
{
    size_t i;

    i = 0;
    if (pthread_create(&monitor->thread, NULL, &check_for_death, monitor) == -1)
        return (EXIT_FAILURE);
    while (i < philos->param->number_of_philosophers)
    {
        if (pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]) == -1)
            return (EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}

int handle_one_philo(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->right_fork->lock);
    write_state("has taken a fork", philo, false);
    pthread_mutex_unlock(&philo->right_fork->lock);
    ft_usleep(philo->param->time_to_die);
    write_state("died", philo, true);
    return (EXIT_SUCCESS);
}

int threads_join(t_philosopher *philos, t_monitor *monitor)
{
    size_t i;
    size_t philo_number;

    i = 0;
    philo_number = philos[i].param->number_of_philosophers;
    pthread_join(monitor->thread, NULL);
    while (i < philo_number)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    return (EXIT_SUCCESS);
}
