#include "../includes/philo.h"

int main (int ac, char **av)
{
    t_philosopher *philos;
    t_supervisor *supervisor;
    t_param param;
    t_fork *forks;

    philos = NULL;
    forks = NULL;
    if (get_data(ac, av, &param))
        return EXIT_FAILURE;
    // INIT DATA
    if (param_init(&param))
        return (EXIT_FAILURE);
    if (forks_init(&forks, &param))
        return (EXIT_FAILURE);
    if (philosophers_init(&philos, forks, &param))
        return (EXIT_FAILURE);
    if (param.number_of_philosophers == 1)
    {
        handle_one_philo(philos);
        return(EXIT_SUCCESS);
    }
    supervisor = malloc(sizeof(t_supervisor));
    supervisor->param = &param;
    supervisor->philos = philos;
    if (start_sumulation(philos, supervisor))
        return (EXIT_FAILURE);
    if (threads_join(philos, supervisor))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}