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
    if (get_time() - philo->last_meal_time > philo->param->time_to_die)
        is_dead = true;
    pthread_mutex_unlock(&philo->lock);
    return (is_dead);
}

int check_philos_condition(t_philosopher *philos)
{
    size_t itr;
    t_param *tmp_param;
    size_t finished;

    itr = 0;
    finished = 0;
    tmp_param = philos[0].param;
    while (itr < tmp_param->number_of_philosophers)
    {
        if (get_status(&philos[itr]) == FULL)
          finished++;
        else if (is_philo_dead(&philos[itr]) == true )
        {
            change_status(&philos[itr], DIED);
            write_state("died", &philos[itr], true);
            philos[itr].last_msg = true;
            pthread_mutex_lock(&tmp_param->is_dead_lock);
            tmp_param->is_dead = true;
            pthread_mutex_unlock(&tmp_param->is_dead_lock);
            return(1);
        }
        itr++;
    }
    if (finished == philos->param->number_of_philosophers)
      return (1);
    return (0);
}

// CHECK IF A PHILOSOPHER DIED
void  *check_for_death(void *data)
{
    t_monitor *s;

    s = (t_monitor *)data;
    while (true)
    {
        if (check_philos_condition(s->philos))
            break;
        usleep(50);
    }
    return (NULL);
}
