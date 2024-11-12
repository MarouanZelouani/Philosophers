/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:20:59 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 02:52:56 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	is_dead(t_philo *philo)
{
	bool	is_dead;

	is_dead = false;
	pthread_mutex_lock(&philo->args->is_dead_lock);
	if (philo->args->is_dead == true)
		is_dead = true;
	pthread_mutex_unlock(&philo->args->is_dead_lock);
	return (is_dead);
}

bool	is_philo_dead(t_philo *philo)
{
	bool	is_dead;

	is_dead = false;
	pthread_mutex_lock(&philo->lock);
	if (get_time() - philo->last_meal_time > philo->args->time_to_die)
		is_dead = true;
	pthread_mutex_unlock(&philo->lock);
	return (is_dead);
}

int	check_philos_condition(t_philo *philos)
{
	size_t	itr;
	size_t	finished;

	itr = 0;
	finished = 0;
	while (itr < philos->args->number_of_philosophers)
	{
		if (get_status(&philos[itr]) == FULL)
			finished++;
		else if (is_philo_dead(&philos[itr]) == true)
		{
			change_status(&philos[itr], DIED);
			usleep(10);
			write_state("died", &philos[itr], true);
			pthread_mutex_lock(&philos->args->is_dead_lock);
			philos->args->is_dead = true;
			pthread_mutex_unlock(&philos->args->is_dead_lock);
			return (1);
		}
		itr++;
	}
	if (finished == philos->args->number_of_philosophers)
		return (1);
	return (0);
}

void	*monitor_routine(void *data)
{
	t_monitor	*s;

	s = (t_monitor *)data;
	if (s->args->number_of_philosophers == 1)
	{
		handle_one_philo(s->philos);
		return (NULL);
	}
	while (true)
	{
		if (check_philos_condition(s->philos))
			break ;
		usleep(10);
	}
	return (NULL);
}
