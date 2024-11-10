/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:21 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 15:43:51 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int start_sumulation(t_philosopher *philos, t_monitor *monitor)
{
    size_t i;

    i = 0;
    if (pthread_create(&monitor->thread, NULL, &monitor_routine, monitor) == -1)
        return (EXIT_FAILURE);
    while (i < philos->param->number_of_philosophers)
    {
        if (pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]) == -1)
            return (EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}

int handle_one_philo(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->right_fork->lock);
    write_state("has taken a fork", philo, false);
    pthread_mutex_unlock(&philo->right_fork->lock);
    ft_usleep(philo->param->time_to_die, philo);
    write_state("died", philo, true);
    return (EXIT_SUCCESS);
}

int threads_join(t_philosopher *philos, t_monitor *monitor)
{
    size_t i;
    size_t philo_number;

    i = 0;
    philo_number = philos[i].param->number_of_philosophers;
    pthread_join(monitor->thread, NULL);
    while (i < philo_number)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    return (EXIT_SUCCESS);
}

void cleanup(t_philosopher *philos, t_monitor *monitor, t_param *param, t_fork *forks)
{
    size_t itr;

    itr = 0;
    while (itr < param->number_of_philosophers)
    {
        pthread_mutex_destroy(&philos[itr].meals_lock);
        pthread_mutex_destroy(&philos[itr].left_fork->lock);
        pthread_mutex_destroy(&philos[itr].right_fork->lock);
        pthread_mutex_destroy(&philos[itr].lock);
        pthread_mutex_destroy(&philos[itr].status_lock);         
        itr++;
    }
    pthread_mutex_destroy(&param->is_dead_lock);
    pthread_mutex_destroy(&param->write_lock);
    free(philos);
    free(forks);
    free(monitor);         
}
