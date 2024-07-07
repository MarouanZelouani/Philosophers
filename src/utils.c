#include "../includes/philo.h"

long get_time(void)
{
    struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void change_status(t_philosopher *philo, enum status status)
{
    pthread_mutex_lock(&philo->status_lock);
    if (philo->status != DIED)
        philo->status = status;
    pthread_mutex_unlock(&philo->status_lock);
}

void	ft_usleep(long time_to_sleep)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time_to_sleep)
		usleep(time_to_sleep / 10);
}

enum status get_status(t_philosopher *philo)
{
    enum status status;
    
    pthread_mutex_lock(&philo->status_lock);
    status = philo->status;
    pthread_mutex_unlock(&philo->status_lock);
    return (status);
}

void write_state(char *s, t_philosopher *philo)
{
    pthread_mutex_lock(&philo->param->write_lock);
    if (philo->last_msg == false)
        printf("%lu %d %s\n", get_time() - philo->param->start_time, philo->id, s);
    pthread_mutex_unlock(&philo->param->write_lock);
}