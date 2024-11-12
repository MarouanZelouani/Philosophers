/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:16 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 00:50:58 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    take_forks(t_philo *philo);
int     examine_death(t_philo *philo);
int     eat_routine(t_philo *philo);

void *routine (void *data)
{
    t_philo *philo;
    
    philo = (t_philo *)data;
    while (1)
    {    
        if (eat_routine(philo))
            break;
        if (examine_death(philo))
            break;
        write_state("is sleeping", philo, false);
        change_status(philo, SLEEPING);
        gosleep(philo->args->time_to_sleep, philo);
        change_status(philo, THINKING);
        if (examine_death(philo))
            break;
        write_state("is thinking", philo, false);
        if (examine_death(philo))
            break;
        usleep(50);
        pthread_mutex_lock(&philo->meals_lock);
        if (philo->number_of_meals >= philo->args->number_of_meals 
        && philo->args->number_of_meals != -1)
        {
            change_status(philo, FULL);
            pthread_mutex_unlock(&philo->meals_lock);
            break;
        }
        pthread_mutex_unlock(&philo->meals_lock);
        if (examine_death(philo))
            break;
    }
    return (NULL);
}


int examine_death(t_philo *philo)
{
    if (is_dead(philo))
        return(1);
    if (is_philo_dead(philo))
    {
        change_status(philo, DIED);
        write_state("died", philo, true);
        pthread_mutex_lock(&philo->args->is_dead_lock);
        philo->args->is_dead = true;
        pthread_mutex_unlock(&philo->args->is_dead_lock);
        return(1);
    };
    return(0);
}

void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0 && is_philo_dead(philo) == false)
    {
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taken a fork", philo, false);
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taken a fork", philo, false);
    }
    else if (philo->id % 2 != 0 && is_philo_dead(philo) == false)
    {
        usleep(100);
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taken a fork", philo, false);
        usleep(50);
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taken a fork", philo, false);
    }
}

int eat_routine(t_philo *philo)
{
    take_forks(philo);
    pthread_mutex_lock(&philo->lock);
    philo->last_meal_time = get_time();
    pthread_mutex_unlock(&philo->lock);
    if (examine_death(philo))
    {
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));
        return (1);
    }
    change_status(philo, EATING);
    write_state("is eating", philo, false);
    gosleep(philo->args->time_to_eat, philo);
    if (examine_death(philo))
    {
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));
        return (1);
    }
    pthread_mutex_lock(&philo->meals_lock);
    philo->number_of_meals++;
    pthread_mutex_unlock(&philo->meals_lock);    
    pthread_mutex_unlock(&(philo->left_fork->lock));
    pthread_mutex_unlock(&(philo->right_fork->lock));
    return (0);
}
