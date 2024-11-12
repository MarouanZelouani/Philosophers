/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:03 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 00:49:03 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int args_init(t_args *args);
int forks_init(t_fork **forks, t_args *args);
int philosophers_init(t_philo **philos, t_fork *forks, t_args *args);

int __init(t_philo **philos, t_fork **forks, t_args *args, t_monitor **monitor)
{
    if (args_init(args))
        return (EXIT_FAILURE);
    if (forks_init(forks, args))
        return (EXIT_FAILURE);
    if (philosophers_init(philos, *forks, args))
        return (EXIT_FAILURE);
    *monitor = malloc(sizeof(t_monitor));
    if (*monitor == NULL)
        return (EXIT_FAILURE);
    (*monitor)->args = args;
    (*monitor)->philos = *philos;
  return (EXIT_SUCCESS);
}

int args_init(t_args *args)
{
    args->is_dead = false;
    args->start_time = get_time();
    if (pthread_mutex_init(&args->write_lock, NULL))
        return (EXIT_FAILURE);
    if (pthread_mutex_init(&args->is_dead_lock, NULL))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int forks_init(t_fork **forks, t_args *args)
{
    size_t i;

    i = 0;
    *forks = malloc (sizeof(t_fork) * args->number_of_philosophers);
    if (*forks == NULL)
        return (EXIT_FAILURE);
    while (i < args->number_of_philosophers)
    {
        if (pthread_mutex_init(&(*forks)[i].lock, NULL))
            return (EXIT_FAILURE);
        (*forks)[i].id = i;
        (*forks)[i].is_availble = 1;
        i++;
    }
    return (EXIT_SUCCESS);
}

int philosophers_init(t_philo **philos, t_fork *forks, t_args *args)
{
    size_t i;

    i = 0;
    *philos = malloc (sizeof(t_philo) * args->number_of_philosophers);
    if (*philos == NULL)
        return (EXIT_FAILURE);
    while (i < args->number_of_philosophers)
    {
        (*philos)[i].id = i + 1;
        (*philos)[i].number_of_meals = 0;
        (*philos)[i].last_meal_time = args->start_time;
        (*philos)[i].right_fork = &forks[i];
        (*philos)[i].left_fork = &forks[(i + 1) % args->number_of_philosophers];
        (*philos)[i].args = args;
        if (pthread_mutex_init(&((*philos)[i].status_lock), NULL))
            return (EXIT_FAILURE);
        if (pthread_mutex_init(&((*philos)[i].lock), NULL))
            return (EXIT_FAILURE);
        if (pthread_mutex_init(&((*philos)[i].meals_lock), NULL))
            return (EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}
