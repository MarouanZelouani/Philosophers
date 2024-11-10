/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:13 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 17:39:29 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    size_t i;
    size_t j;

    (void)ac;
    i = 1;
    while (av[i])
    {
        j = 0;
        while (av[i][j])
        {
            if (!ft_isdigit(av[i][j]))
                return(i);
            j++;
        }
        i++;
    }
    return (0);
}


static int check_numbers(t_args *args)
{
    if (args == NULL)
        return (EXIT_FAILURE);
    if (args->number_of_philosophers <= 0)
        return (EXIT_FAILURE);
    if (args->number_of_meals == 0 || args->number_of_meals < -1)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int get_data(int ac, char **av, t_args *args)
{
    int error;

    if (ac != 5 && ac != 6)
        _error(0, av);
    error = check_arguments(ac, av);
    if (error != 0)
        _error(error, av);
    args->number_of_philosophers = ft_atoi(av[1]);
    args->time_to_die = ft_atoi(av[2]);
    args->time_to_eat = ft_atoi(av[3]);
    args->time_to_sleep = ft_atoi(av[4]);
    args->number_of_meals = -1;
    if (ac == 6)
        args->number_of_meals = ft_atoi(av[5]);
    if (check_numbers(args))
        return (EXIT_FAILURE);
    return(EXIT_SUCCESS);
}
