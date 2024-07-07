#include "../includes/philo.h"

int main (int ac, char **av)
{
    t_philosopher *philos;
    t_supervisor supervisor;
    t_param param;
    t_fork *forks;

    philos = NULL;
    forks = NULL;
    get_data(ac, av, &param);
    if (!data_init(&param, philos, forks, &supervisor))
        return (EXIT_FAILURE);
    printf("1\n");
    if (!start_sumulation(philos, supervisor))
        return (EXIT_FAILURE);
    if (threads_join(philos, supervisor))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}