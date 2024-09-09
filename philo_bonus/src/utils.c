#include "../includes/philo_bonus.h"

long get_time(void)
{
    struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long time_to_sleep)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time_to_sleep)
		usleep(time_to_sleep / 10);
}

void write_state(char *s, t_philosopher *philo)
{
    sem_wait(philo->param->write_sem);
    if (philo->last_msg == false)
        printf("%lu %d %s\n", get_time() - philo->param->start_time, philo->id, s);
    sem_post(philo->param->write_sem);
}