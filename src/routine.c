/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:16 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 04:37:06 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    take_forks(t_philosopher *philo);
int     examine_death(t_philosopher *philo);
int     eat_routine(t_philosopher *philo);

void *routine (void *data)
{
    t_philosopher *philo;
    
    philo = (t_philosopher *)data;
    while (is_dead(philo) == false)
    {    
        if (examine_death(philo)) // DEATH CHECK
            break;
        if (eat_routine(philo)) // EATING
            break;
        if (examine_death(philo)) // DEATH CHECK
            break;
        write_state("is sleeping", philo, false);
        change_status(philo, SLEEPING);
        ft_usleep(philo->param->time_to_sleep, philo);
        change_status(philo, THINKING);
        if (examine_death(philo)) // DEATH CHECK
            break;
        write_state("is thinking", philo, false);
        usleep(50);
        if (examine_death(philo)) // DEATH CHECK
            break;
        pthread_mutex_lock(&philo->meals_lock);
        if (philo->number_of_meals >= philo->param->number_of_meals 
        && philo->param->number_of_meals != -1)
        {
            change_status(philo, FULL);
            pthread_mutex_unlock(&philo->meals_lock);
            break;
        }
        pthread_mutex_unlock(&philo->meals_lock);
    }
    return (NULL);
}


int examine_death(t_philosopher *philo)
{
    if (is_dead(philo))
        return(1);
    if (is_philo_dead(philo))
    {
        change_status(philo, DIED);
        write_state("died", philo, true);
        philo->last_msg = true;
        pthread_mutex_lock(&philo->param->is_dead_lock);
        philo->param->is_dead = true;
        pthread_mutex_unlock(&philo->param->is_dead_lock);
        return(1);
    };
    return(0);
}

void take_forks(t_philosopher *philo)
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
        usleep(100);
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taken a fork", philo, false);
    }
}

int eat_routine(t_philosopher *philo)
{
    take_forks(philo);
    pthread_mutex_lock(&philo->lock);
    philo->last_meal_time = get_time();
    pthread_mutex_unlock(&philo->lock);
    if (examine_death(philo)) // DEATH CHECK
    {
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));
        return (1);
    }
    change_status(philo, EATING); // change the status
    write_state("is eating", philo, false);
    ft_usleep(philo->param->time_to_eat, philo);
    if (examine_death(philo)) // DEATH CHECK
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
