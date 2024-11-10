/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:03 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 15:35:27 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int param_init(t_param *param);
int forks_init(t_fork **forks, t_param *param);
int philosophers_init(t_philosopher **philos, t_fork *forks, t_param *param);

int __init(t_philosopher **philos, t_fork **forks, t_param *param, t_monitor **monitor)
{
    if (param_init(param))
        return (EXIT_FAILURE);
    if (forks_init(forks, param))
        return (EXIT_FAILURE);
    if (philosophers_init(philos, *forks, param))
        return (EXIT_FAILURE);
    *monitor = malloc(sizeof(t_monitor));
    if (*monitor == NULL)
        return (EXIT_FAILURE);
    (*monitor)->param = param;
    (*monitor)->philos = *philos;
  return (EXIT_SUCCESS);
}

// INIT DATA
int param_init(t_param *param)
{
    param->is_dead = false;
    param->start_time = get_time(); // START
    // ALLWAYS INIT YOUR MUTEXS!!
    if (pthread_mutex_init(&param->write_lock, NULL) == -1)
        return (EXIT_FAILURE);
    if (pthread_mutex_init(&param->is_dead_lock, NULL) == -1)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

// INIT FORKS
int forks_init(t_fork **forks, t_param *param)
{
    size_t i;

    i = 0;
    *forks = malloc (sizeof(t_fork) * param->number_of_philosophers);
    if (*forks == NULL)
        return (EXIT_FAILURE);
    while (i < param->number_of_philosophers)
    {
        if (pthread_mutex_init(&(*forks)[i].lock, NULL) == -1)
            return (EXIT_FAILURE);
        (*forks)[i].id = i;
        (*forks)[i].is_availble = 1;
        i++;
    }
    return (EXIT_SUCCESS);
}

// INIT PHILOSOPHERS
int philosophers_init(t_philosopher **philos, t_fork *forks, t_param *param)
{
    size_t i;

    i = 0;
    *philos = malloc (sizeof(t_philosopher) * param->number_of_philosophers);
    if (*philos == NULL)
        return (EXIT_FAILURE);
    while (i < param->number_of_philosophers)
    {
        (*philos)[i].id = i + 1;
        (*philos)[i].number_of_meals = 0;
        (*philos)[i].last_msg = false;
        (*philos)[i].last_meal_time = param->start_time;
        (*philos)[i].right_fork = &forks[i];
        (*philos)[i].left_fork = &forks[(i + 1) % param->number_of_philosophers];
        (*philos)[i].param = param;
        if (pthread_mutex_init(&((*philos)[i].status_lock), NULL) == -1)
            return (EXIT_FAILURE);
        if (pthread_mutex_init(&((*philos)[i].lock), NULL) == -1)
            return (EXIT_FAILURE);
        if (pthread_mutex_init(&((*philos)[i].meals_lock), NULL) == -1)
            return (EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}
