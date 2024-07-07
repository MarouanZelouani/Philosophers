#include "../includes/philo.h"

int take_forks(t_philosopher *philo)
{
    if (philo->id % 2 == 0 && is_philo_dead(philo) == false)
    {
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taking a fork", philo);
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taking a fork", philo);
        return (EXIT_SUCCESS);
    }
    else if (philo->id % 2 != 0 && is_philo_dead(philo) == false)
    {
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taking a fork", philo);
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taking a fork", philo);
        return (EXIT_SUCCESS);
    }
    return (EXIT_SUCCESS);
}

void *routine (void *data)
{
    t_philosopher *philo;
    bool dead;
    
    philo = (t_philosopher *)data;
    dead = false;
    while (!dead && is_dead(philo) == false)
    {   
        // EATING
        take_forks(philo);
        change_status(philo, EATING); // change the status
        write_state("is eating", philo);
        ft_usleep(philo->param->time_to_eat);
        
        // number_of_meals
        pthread_mutex_lock(&philo->meals_lock);
        philo->number_of_meals++;
        pthread_mutex_unlock(&philo->meals_lock);
        
        // last_meal_time
        pthread_mutex_lock(&philo->lock);
        philo->last_meal_time = get_time();
        pthread_mutex_unlock(&philo->lock);

        // RELEASE FORKS
        philo->left_fork->is_availble = 0;
        philo->right_fork->is_availble = 0; 
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));

        //  SLEEPING
        change_status(philo, SLEEPING);
        write_state("is sleeping", philo);
        ft_usleep(philo->param->time_to_sleep);

        // THINKING
        change_status(philo, THINKING);
        write_state("is thinking", philo);
        pthread_mutex_lock(&philo->meals_lock);
        if (philo->number_of_meals >= MAX_MEALS && MAX_MEALS != -1)
            dead = true;
        pthread_mutex_unlock(&philo->meals_lock);
    }
    return (NULL);
}