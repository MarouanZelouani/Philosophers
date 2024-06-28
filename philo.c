#include "./includes/philo.h"

// ROUTINE FUNCTION
void *routine1 (void *data)
{
    t_philosopher *philo;
    
    philo = (t_philosopher *)data;
    while (1)
    {
        if (philo->left_fork->is_availble == 0 
            && philo->right_fork->is_availble == 0)
        {
            pthread_mutex_lock(&(philo->left_fork->lock));
            printf("%d has taking a fork\n", philo->id);
            philo->left_fork->is_availble = 1;
            pthread_mutex_lock(&(philo->right_fork->lock));
            printf("%d has taking a fork\n", philo->id);
            philo->right_fork->is_availble = 1;
            printf("%d is eating\n", philo->id);
            usleep(TIME_TO_EAT);
            philo->number_of_meals++;
            pthread_mutex_unlock(&(philo->left_fork->lock));
            pthread_mutex_unlock(&(philo->right_fork->lock));
            philo->left_fork->is_availble = 0;
            philo->right_fork->is_availble = 0; 
            printf("%d is sleeping\n", philo->id);
            usleep(TIME_TO_SLEEP);
            printf("%d is thinking\n", philo->id);
        }
    }
    return (NULL);
}



long get_time(void)
{
    struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int is_threads_created(t_philosopher *philo)
{
    int ret;

    ret = 0;
    pthread_mutex_lock(&philo->param->threads_ready_lock);
    if (philo->param->threads_ready == 1)
        ret = 1;
    pthread_mutex_unlock(&philo->param->threads_ready_lock);
    return(ret);
}

void write_state(char *s, t_philosopher *philo)
{
    pthread_mutex_lock(&philo->param->write_lock);
    printf("%ld %d %s\n", get_time() - philo->param->start_time, philo->id, s);
    pthread_mutex_unlock(&philo->param->write_lock);
}

void *routine (void *data)
{
    t_philosopher *philo;
    philo = (t_philosopher *)data;
    // WAIT FOR ALL THE THREAD TO BE GREATED
    //while (is_threads_created(philo) == 0);
    printf("wfsjdf\n");
    while (1)
    {
        if (philo->number_of_meals >= philo->param->number_of_meals
            && philo->param->number_of_meals != -1
            && philo->left_fork->is_availble ==  1
            && philo->right_fork->is_availble == 1)
        {
            //  EATING
            pthread_mutex_lock(&(philo->left_fork->lock));
            write_state("has taking a fork", philo);
            philo->left_fork->is_availble = 0;
            pthread_mutex_lock(&(philo->right_fork->lock));
            write_state("has taking a fork", philo);
            philo->right_fork->is_availble = 1;
            write_state("is eating", philo);
            usleep(TIME_TO_EAT);
            philo->number_of_meals++;
            // RELEASE FORKS
            philo->left_fork->is_availble = 0;
            philo->right_fork->is_availble = 0; 
            pthread_mutex_unlock(&(philo->left_fork->lock));
            pthread_mutex_unlock(&(philo->right_fork->lock));

            //  SLEEPING
            write_state("is sleeping", philo);
            usleep(TIME_TO_SLEEP);

            //  THINKING
            write_state("is thinking", philo);
        }
    }
    
    return (NULL);
}

int main (int ac, char **av)
{
    t_philosopher *philos;
    t_fork *forks;
    t_param param;
    int i;

    // INIT DATA
        // ALLOCATION
    philos = malloc (sizeof(t_philosopher) * NUMBER_OF_PHILOS);
    forks = malloc (sizeof(t_fork) * NUMBER_OF_PHILOS);
    param.threads_ready = 0;

        // INIT FORKS
    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        pthread_mutex_init(&(forks[i].lock), NULL);
        forks[i].id = i;
        forks[i].is_availble = 1;
        i++;
    }

        // INIT PHILOSOPHERS
    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        philos[i].id = i + 1;
        philos[i].number_of_meals = -1;
        // ASSIGN FORKS
        philos[i].right_fork = &forks[i];
        philos[i].left_fork = &forks[(i + 1) % NUMBER_OF_PHILOS];
        philos[i].param = &param; 
        i++;
    }

    // START SIMULATION
        // CHECK IF THE NUMBER_OF_MEALS IF SET TO 0 !!
    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]);
        i++;
    }
    
        // ALL THREADS HAVE BEEN CREATED
    param.start_time = 0;      
    pthread_mutex_lock(&param.threads_ready_lock);
    param.threads_ready = 1;
    pthread_mutex_unlock(&param.threads_ready_lock);


        // JOINING THREADS
    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    exit(0);
}