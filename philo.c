#include "./includes/philo.h"

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
    pthread_mutex_lock(&philo->param->write_lock);
    printf("%lu %d %s\n", get_time() - philo->param->start_time, philo->id, s);
    pthread_mutex_unlock(&philo->param->write_lock);
}

void  *check_for_death(void *data)
{
    t_supervisor *s;
    int i;

    s = (t_supervisor *)data;
    t_philosopher **philos = (t_philosopher **)s->philos;
    // CHECK IF A PHILOSOPHER IS DEAD
    while (1)
    {
        i = 0;
        while(philos[i])
        {
            if (philos[i]->status == THINKING
            && get_time() - philos[i]->last_meal_time >= TIME_TO_DIE
            && philos[i]->last_meal_time != EATING)
            {
                
            }
            i++;
        }
    }
    return (NULL);
}

void *routine (void *data)
{
    t_philosopher *philo;
    
    philo = (t_philosopher *)data;
    
    
    // WAIT FOR ALL THE THREAD TO BE GREATED
    //while (philo->param->threads_ready == 0){}
    
    // THE START OF THE EXPERIMENT
    if (philo->param->launch == false)
    {
        pthread_mutex_lock(&philo->param->launch_lock);
        philo->param->start_time = get_time();
        //printf("%lu\n", philo->param->start_time);
        philo->param->launch = true;
        pthread_mutex_unlock(&philo->param->launch_lock);
    }

    if (philo->id % 2 != 0)
        ft_usleep(TIME_TO_EAT);
    
    while (1)
    {   
        // EATING
        pthread_mutex_lock(&(philo->left_fork->lock));
        write_state("has taking a fork", philo);
        philo->left_fork->is_availble = 0;
        pthread_mutex_lock(&(philo->right_fork->lock));
        write_state("has taking a fork", philo);
        philo->right_fork->is_availble = 1;
        write_state("is eating", philo);
        philo->status = EATING; // change the status
        ft_usleep(TIME_TO_EAT);
        philo->number_of_meals++;
        philo->last_meal_time = get_time();

        // RELEASE FORKS
        philo->left_fork->is_availble = 0;
        philo->right_fork->is_availble = 0; 
        pthread_mutex_unlock(&(philo->left_fork->lock));
        pthread_mutex_unlock(&(philo->right_fork->lock));

        //  SLEEPING
        write_state("is sleeping", philo);
        philo->status = SLEEPING;
        ft_usleep(TIME_TO_SLEEP);

        // THINKING
        philo->status = THINKING;
        write_state("is thinking", philo);
        // if (philo->number_of_meals == MAX_MEALS)
        //     break;
    }
    return (NULL);
}

int main (int ac, char **av)
{
    t_philosopher *philos;
    t_fork *forks;
    t_param param;
    t_supervisor supervisor;
    int i;

    // INIT DATA
        // ALLOCATION
    philos = malloc (sizeof(t_philosopher) * NUMBER_OF_PHILOS);
    forks = malloc (sizeof(t_fork) * NUMBER_OF_PHILOS);
    param.threads_ready = 0;
    param.launch = false;
    param.start_time = 0; // START     
    // ALLWAYS INIT YOUR MUTEXS !!
    pthread_mutex_init(&param.write_lock, NULL);
    pthread_mutex_init(&param.threads_ready_lock, NULL);
    pthread_mutex_init(&param.launch_lock, NULL);


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
        philos[i].number_of_meals = 0;
        philos[i].status = STARTING;
        // ASSIGN FORKS
        philos[i].right_fork = &forks[i];
        philos[i].left_fork = &forks[(i + 1) % NUMBER_OF_PHILOS];
        philos[i].param = &param; 
        i++;
    }

    // INIT SUPERVISOR
    supervisor.param = &param;
    supervisor.philos = &philos;
    pthread_create(&supervisor.thread, NULL, &check_for_death, &supervisor);

    // START SIMULATION
        // CHECK IF THE NUMBER_OF_MEALS IF SET TO 0 !!
    i = 0;
    while (i < NUMBER_OF_PHILOS)
    {
        pthread_create(&(philos[i].thread), NULL, &routine, &philos[i]);
        i++;
    }
        // ALL THREADS HAVE BEEN CREATED
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
    pthread_join(supervisor.thread, NULL);
}