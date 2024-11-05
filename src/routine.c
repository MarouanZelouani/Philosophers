#include "../includes/philo.h"

int yeah_check_for_death(t_philosopher *philo)
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

int take_forks(t_philosopher *philo)
{
    if (philo->id % 2 == 0 && is_philo_dead(philo) == false)
    {
        pthread_mutex_lock(&philo->left_fork->lock);
        // if (yeah_check_for_death(philo)) // DEATH CHECK
        // {
        //     pthread_mutex_unlock(&(philo->left_fork->lock));
        //     return (1);
        // }
        write_state("has taken a fork", philo, false);
        pthread_mutex_lock(&philo->right_fork->lock);
        // if (yeah_check_for_death(philo)) // DEATH CHECK
        // {
        //     pthread_mutex_unlock(&(philo->left_fork->lock));
        //     pthread_mutex_unlock(&(philo->right_fork->lock));
        //     return (1);
        // }
        write_state("has taken a fork", philo, false);
    }
    else if (philo->id % 2 != 0 && is_philo_dead(philo) == false)
    {
        usleep(300);
        pthread_mutex_lock(&philo->right_fork->lock);
        // if (yeah_check_for_death(philo)) // DEATH CHECK
        // {
        //     pthread_mutex_unlock(&(philo->right_fork->lock));
        //     return (1);
        // }
        write_state("has taken a fork", philo, false);
        pthread_mutex_lock(&philo->left_fork->lock);
        // if (yeah_check_for_death(philo)) // DEATH CHECK
        // {
        //     pthread_mutex_unlock(&(philo->right_fork->lock));
        //     pthread_mutex_unlock(&(philo->left_fork->lock));
        //     return (1);
        // }
        write_state("has taken a fork", philo, false);
    }
    return (0);
}


int eat_routine(t_philosopher *philo)
{
    if(take_forks(philo))
        return (1);
    pthread_mutex_lock(&philo->lock);
    philo->last_meal_time = get_time();
    pthread_mutex_unlock(&philo->lock);

    //DEATH CHECK
    if (yeah_check_for_death(philo)) // DEATH CHECK
    {
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));
        return (1);
    }
    
    change_status(philo, EATING); // change the status
    write_state("is eating", philo, false);
    ft_usleep(philo->param->time_to_eat);
    if (yeah_check_for_death(philo)) // DEATH CHECK
    {
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));
        return (1);
    }
    // number_of_meals
    pthread_mutex_lock(&philo->meals_lock);
    philo->number_of_meals++;
    pthread_mutex_unlock(&philo->meals_lock);    

    // RELEASE FORKS
    pthread_mutex_unlock(&(philo->left_fork->lock));
    pthread_mutex_unlock(&(philo->right_fork->lock));
    return (0);
}

void *routine (void *data)
{
    t_philosopher *philo;
    bool dead;
    
    philo = (t_philosopher *)data;
    dead = false;
    while (!dead && is_dead(philo) == false)
    {   
        
        if (yeah_check_for_death(philo)) // DEATH CHECK
            break;

        if (eat_routine(philo)) // EATING
            break;

        if (yeah_check_for_death(philo)) // DEATH CHECK
            break;

        //  SLEEPING
        write_state("is sleeping", philo, false);
        change_status(philo, SLEEPING);
        ft_usleep(philo->param->time_to_sleep);


        // THINKING
        change_status(philo, THINKING);
        if (yeah_check_for_death(philo)) // DEATH CHECK
            break;
        write_state("is thinking", philo, false);
        if (yeah_check_for_death(philo)) // DEATH CHECK
            break;
        pthread_mutex_lock(&philo->meals_lock);
        if (philo->number_of_meals >= philo->param->number_of_meals 
        && philo->param->number_of_meals != -1)
        {
            change_status(philo, FULL);
            dead = true;
        }
        pthread_mutex_unlock(&philo->meals_lock);
    }
    return (NULL);
}