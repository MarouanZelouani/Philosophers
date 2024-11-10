#include "../includes/philo.h"

int main (int ac, char **av)
{
    t_philosopher *philos;
    t_monitor *monitor;
    t_param param;
    t_fork *forks;

    philos = NULL;
    forks = NULL;
    if (get_data(ac, av, &param)) //  ARGS PARSING
        return (EXIT_FAILURE);
    if (__init(&philos, &forks, &param, &monitor)) // INIT DATA
      return (EXIT_FAILURE);
    if (1 == param.number_of_philosophers) // CASE : ONE PHILO
        return (handle_one_philo(philos));
    if (start_sumulation(philos, monitor)) // CREATE THREADS
        return (EXIT_FAILURE);
    if (threads_join(philos, monitor)) // JOIN THREADS PHLOS AND MONITOR
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}
