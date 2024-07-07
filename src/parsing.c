#include "../includes/philo.h"

void _error(int code, char **av)
{
    if (code == 0)
    {
        printf("Usage : ./philo <arguments..>\n");
        printf("\t [1] - number_of_philosophers\n");
        printf("\t [2] - time_to_die\n");
        printf("\t [3] - time_to_eat\n");
        printf("\t [4] - time_to_sleep\n");
        printf("\t [optional] : number_of_times_each_philosopher_must_eat\n");
    }
    else if (code > 0)
        printf("%s => is not a valid argument!!\n", av[code]);
    exit(EXIT_FAILURE);
}

int check_arguments(int ac, char **av)
{
    int i;
    int j;

    (void)ac;
    i = 1;
    while (av[i])
    {
        j = 0;
        while (av[i][j])
        {
            if (ft_isdigit(av[i][j]))
                return(i);
            j++;
        }
        i++;
    }
    return (0);
}

int get_data(int ac, char **av, t_param *param)
{
    int error;
    
    if (ac != 4 || ac != 5)
        _error(0, av);
    error = check_arguments(ac, av);
    if (error != 0)
        _error(error, av);
    param->number_of_philosophers = ft_atoi(av[1]);
    param->time_to_die = ft_atoi(av[2]);
    param->time_to_eat = ft_atoi(av[3]);
    param->time_to_sleep = ft_atoi(av[4]);
    param->number_of_meals = -1;
    if (ac == 5)
        param->number_of_meals = ft_atoi(av[5]);
    return(EXIT_SUCCESS);   
}