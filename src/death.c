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
    if (get_time() - philo->last_meal_time >= philo->param->time_to_die)
        is_dead = true;
    pthread_mutex_unlock(&philo->lock);
    return (is_dead);
}

// bool dinner_end(t_philosopher *philos)
// {
//     size_t n;
//     size_t i;

//     n = 0;
//     i = 0;
//     while (i < philos->param->number_of_philosophers)
//     {
//         pthread_mutex_lock(&philos[i].meals_lock);
//         if (philos[i].number_of_meals >= philos->param->number_of_meals
//         && philos->param->number_of_meals != -1)
//             n++;
//         pthread_mutex_unlock(&philos[i].meals_lock);
//         i++;
//     }
//     if(n >= philos->param->number_of_philosophers || is_dead(&philos[0]))
//         return (true);
//     return (false);
// }

int check_philos_condition(t_philosopher *philos, size_t *finished)
{
    size_t itr;
    t_param *tmp_param;


    itr = 0;
    tmp_param = philos[0].param;
    while (itr < tmp_param->number_of_philosophers)
    {
        if (get_status(&philos[itr]) == FULL)
            *finished++;
        else if (is_philo_dead(&philos[itr]) == true )
        {
            change_status(&philos[itr], DIED);
            write_state("died", &philos[itr]);
            philos[itr].last_msg = true;
            pthread_mutex_lock(&tmp_param->is_dead_lock);
            tmp_param->is_dead = true;
            pthread_mutex_unlock(&tmp_param->is_dead_lock);
            return(1);
        }
        itr++;
    }
    return (0);
}

// CHECK IF A PHILOSOPHER DIED
void  *check_for_death(void *data)
{
    t_supervisor *s;
    size_t finished_philos;

    s = (t_supervisor *)data;
    finished_philos = 0;
    while (true)
    {
        if (finished_philos == s->param->number_of_philosophers)
            break;
        if (check_philos_condition(s->philos, &finished_philos))
            break;
    }
    return (NULL);
}