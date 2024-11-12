/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:21 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 00:51:10 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int start_sumulation(t_philo *philos, t_monitor *monitor)
{
    size_t i;

    i = 0;
    while (i < philos->args->number_of_philosophers)
    {
        if (philos->args->number_of_philosophers == 1)
            break;
        if (pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]))
            return (EXIT_FAILURE);
        i++;
    }
    if (pthread_create(&monitor->thread, NULL, &monitor_routine, monitor))
        return (EXIT_FAILURE);
    if (threads_join(philos, monitor))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int handle_one_philo(t_philo *philo)
{
    
    pthread_mutex_lock(&philo->right_fork->lock);
    write_state("has taken a fork", philo, false);
    pthread_mutex_unlock(&philo->right_fork->lock);
    gosleep(philo->args->time_to_die, philo);
    write_state("died", philo, true);
    return (EXIT_SUCCESS);
}

int threads_join(t_philo *philos, t_monitor *monitor)
{
    size_t i;
    size_t philo_number;

    i = 0;
    philo_number = philos[i].args->number_of_philosophers;
    pthread_join(monitor->thread, NULL);
    while (i < philo_number)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    return (EXIT_SUCCESS);
}

void cleanup(t_philo *philos, t_monitor *monitor, t_args *args, t_fork *forks)
{
    size_t itr;

    itr = 0;
    while (itr < args->number_of_philosophers)
    {
        pthread_mutex_destroy(&philos[itr].meals_lock);
        pthread_mutex_destroy(&philos[itr].lock);
        pthread_mutex_destroy(&philos[itr].status_lock);
        pthread_mutex_destroy(&forks[itr].lock);        
        itr++;
    }
    pthread_mutex_destroy(&args->is_dead_lock);
    pthread_mutex_destroy(&args->write_lock);
    free(philos);
    free(forks);
    free(monitor);         
}
