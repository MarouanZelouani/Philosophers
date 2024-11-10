/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:24 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 18:01:09 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long get_time(void)
{
    struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void change_status(t_philo *philo, t_status status)
{
    pthread_mutex_lock(&philo->status_lock);
    if (philo->status != DIED)
        philo->status = status;
    pthread_mutex_unlock(&philo->status_lock);
}

void	gosleep(long time_to_sleep, t_philo *philo)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time_to_sleep)
    {
		usleep(time_to_sleep / 10);
        if (is_dead(philo))
            break;
    }
}

t_status get_status(t_philo *philo)
{
    t_status philo_status;

    pthread_mutex_lock(&philo->status_lock);
    philo_status = philo->status;
    pthread_mutex_unlock(&philo->status_lock);
    return (philo_status);
}

void write_state(char *s, t_philo *philo, bool stop)
{
    static bool last_message = false;

    pthread_mutex_lock(&philo->args->write_lock);
    if (last_message == false && philo->last_msg == false && is_dead(philo) == false)
        printf("%lu %d %s\n", get_time() - philo->args->start_time, philo->id, s);
    if (stop == true)
        last_message = true;
    pthread_mutex_unlock(&philo->args->write_lock);
}
