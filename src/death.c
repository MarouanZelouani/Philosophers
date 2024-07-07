#include "../includes/philo.h"

bool is_dead(t_philosopher *philo)
{
    bool is_dead;

    is_dead = false;
    pthread_mutex_lock(&philo->param->is_dead_lock);
    if (philo->param->is_dead == true)
        is_dead = true;
    pthread_mutex_unlock(&philo->param->is_dead_lock);
    return (is_dead);
}

// CHECK IF THE PHILOSOPHER IS DEAD
bool is_philo_dead(t_philosopher *philo)
{
    bool is_dead;

    is_dead = false;
    pthread_mutex_lock(&philo->lock);
    if (get_time() - philo->last_meal_time >= TIME_TO_DIE 
    && philo->status == THINKING)
        is_dead = true;
    pthread_mutex_unlock(&philo->lock);
    return (is_dead);
}

bool dinner_end(t_philosopher *philos)
{
    int n;
    int i;

    n = 0;
    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        pthread_mutex_lock(&philos[i].meals_lock);
        if (philos[i].number_of_meals >= MAX_MEALS
        && MAX_MEALS != -1)
            n++;
        pthread_mutex_unlock(&philos[i].meals_lock);
        i++;
    }
    if(n >= NUMBER_OF_PHILOS || is_dead(&philos[0]))
        return (true);
    return (false);
}

// CHECK IF A PHILOSOPHER DIED
void  *check_for_death(void *data)
{
    t_supervisor *s;
    int i;

    s = (t_supervisor *)data;
    while (dinner_end(s->philos) == false)
    {
        i = 0;
        while (i < NUMBER_OF_PHILOS )
        {
            if (get_status(&s->philos[i]) == THINKING
            && is_philo_dead(&s->philos[i]) == true)
            {
                change_status(&s->philos[i], DIED);
                write_state("died", &s->philos[i]);
                s->philos[i].last_msg = true;
                pthread_mutex_lock(&s->param->is_dead_lock);
                s->param->is_dead = true;
                pthread_mutex_unlock(&s->param->is_dead_lock);
            }
            i++;
        }
    }
    return (NULL);
}