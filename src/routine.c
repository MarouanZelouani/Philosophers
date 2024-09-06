#include "../includes/philo.h"

void take_forks(t_philosopher *philo)
{
    if (philo->id % 2 == 0 && is_philo_dead(philo) == false)
    {
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taken a fork", philo);
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taken a fork", philo);
    }
    else if (philo->id % 2 != 0 && is_philo_dead(philo) == false)
    {
        usleep(300);
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taken a fork", philo);
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taken a fork", philo);
    }
}

void *routine (void *data)
{
    t_philosopher *philo;
    bool dead;
    
    philo = (t_philosopher *)data;
    dead = false;
    while (!dead && is_dead(philo) == false)
    {   
        if (is_dead(philo))
            break;
        if (is_philo_dead(philo))
        {
            change_status(philo, DIED);
            write_state("died", philo);
            philo->last_msg = true;
            pthread_mutex_lock(&philo->param->is_dead_lock);
            philo->param->is_dead = true;
            pthread_mutex_unlock(&philo->param->is_dead_lock);
            break;
        }
        // EATING
        take_forks(philo);
        // last_meal_time
        pthread_mutex_lock(&philo->lock);
        philo->last_meal_time = get_time();
        pthread_mutex_unlock(&philo->lock);
        if (is_dead(philo))
        {
            philo->left_fork->is_availble = 0;
            philo->right_fork->is_availble = 0; 
            pthread_mutex_unlock(&(philo->left_fork->lock));
            pthread_mutex_unlock(&(philo->right_fork->lock));
            break;
        }
        if (is_philo_dead(philo))
        {
            philo->left_fork->is_availble = 0;
            philo->right_fork->is_availble = 0; 
            pthread_mutex_unlock(&(philo->left_fork->lock));
            pthread_mutex_unlock(&(philo->right_fork->lock));
            change_status(philo, DIED);
            write_state("died", philo);
            philo->last_msg = true;
            pthread_mutex_lock(&philo->param->is_dead_lock);
            philo->param->is_dead = true;
            pthread_mutex_unlock(&philo->param->is_dead_lock);
            break;
        }
        
        change_status(philo, EATING); // change the status
        write_state("is eating", philo);
        ft_usleep(philo->param->time_to_eat);
        
        // number_of_meals
        pthread_mutex_lock(&philo->meals_lock);
        philo->number_of_meals++;
        pthread_mutex_unlock(&philo->meals_lock);
        
        

        // RELEASE FORKS
        philo->left_fork->is_availble = 0;
        philo->right_fork->is_availble = 0; 
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));
        if (is_dead(philo))
            break;
        if (is_philo_dead(philo))
        {
            change_status(philo, DIED);
            write_state("died", philo);
            philo->last_msg = true;
            pthread_mutex_lock(&philo->param->is_dead_lock);
            philo->param->is_dead = true;
            pthread_mutex_unlock(&philo->param->is_dead_lock);
            break;
        }
        
        //  SLEEPING
        change_status(philo, SLEEPING);
        write_state("is sleeping", philo);
        ft_usleep(philo->param->time_to_sleep);
        if (is_dead(philo))
            break;
        if (is_philo_dead(philo))
        {
            change_status(philo, DIED);
            write_state("died", philo);
            philo->last_msg = true;
            pthread_mutex_lock(&philo->param->is_dead_lock);
            philo->param->is_dead = true;
            pthread_mutex_unlock(&philo->param->is_dead_lock);
            break;
        };
        // THINKING
        change_status(philo, THINKING);
        write_state("is thinking", philo);
        pthread_mutex_lock(&philo->meals_lock);
        if (philo->number_of_meals >= philo->param->number_of_meals && philo->param->number_of_meals != -1)
            dead = true;
        pthread_mutex_unlock(&philo->meals_lock);
        
    }
    return (NULL);
}