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
    if (get_time() - philo->last_meal_time > philo->param->time_to_die
        && philo->status == THINKING)
        is_dead = true;
    pthread_mutex_unlock(&philo->lock);
    return (is_dead);
}

bool dinner_end(t_philosopher *philos)
{
    size_t n;
    size_t i;

    n = 0;
    i = 0;
    while (i < philos->param->number_of_philosophers)
    {
        pthread_mutex_lock(&philos[i].meals_lock);
        if (philos[i].number_of_meals >= philos->param->number_of_meals
        && philos->param->number_of_meals != -1)
            n++;
        pthread_mutex_unlock(&philos[i].meals_lock);
        i++;
    }
    if(n >= philos->param->number_of_philosophers || is_dead(&philos[0]))
        return (true);
    return (false);
}

// CHECK IF A PHILOSOPHER DIED
void  *check_for_death(void *data)
{
    t_supervisor *s;
    size_t i;

    s = (t_supervisor *)data;
    while (dinner_end(s->philos) == false)
    {
        i = 0;
        while (i < s->param->number_of_philosophers )
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