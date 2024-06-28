#include "./includes/philo.h"

int main (int ac, char **av)
{
    t_param param;
    t_philosopher *philosophers;
    t_fork *forks;

    if (ac != 4 || ac != 5)
    {
        printf("Usage : ./philo <arguments..>\n");
        printf("\t [1] - number_of_philosophers\n");
        printf("\t [2] - time_to_die\n");
        printf("\t [3] - time_to_eat\n");
        printf("\t [4] - time_to_sleep\n");
        printf("\t [optional] : number_of_times_each_philosopher_must_eat\n");
        exit(EXIT_FAILURE);
    }
    param.number_of_philosophers = atoi(av[1]);
    param.time_to_die = atoi(av[2]);
    param.time_to_eat = atoi(av[3]);
    param.time_to_sleep = atoi(av[4]);
    param.number_of_meals = -1;
    if (ac == 5)
        param.number_of_meals = atoi(av[5]);
    philosophers = malloc(sizeof(t_philosopher) * param.number_of_philosophers);
    if (philosophers == NULL)
        exit(EXIT_FAILURE);
    forks = malloc(sizeof(t_fork) * param.number_of_philosophers);
    if (forks == NULL)
    {
        free(philosophers);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}