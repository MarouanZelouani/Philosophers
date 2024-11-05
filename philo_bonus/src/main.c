#include "../includes/philo_bonus.h"

int main (int ac, char **av)
{
    t_param *param;
    t_philosopher *philos;
    
    philos = NULL;
    param = NULL;
    if (get_args(ac, av, &param))
        return (EXIT_FAILURE);
    if (init_data(&philos, param))
        return (cleanup(param, philos), EXIT_FAILURE);
    if (start_simulation(philos, param))
        return (cleanup(param, philos), EXIT_FAILURE);
    return (cleanup(param, philos), EXIT_SUCCESS);
}