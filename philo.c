#include "./includes/philo.h"

long get_time(void)
{
    struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void change_status(t_philosopher *philo, enum status status)
{
    pthread_mutex_lock(&philo->status_lock);
    philo->status = status;
    pthread_mutex_unlock(&philo->status_lock);
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

enum status get_status(t_philosopher *philo)
{
    enum status status;
    
    pthread_mutex_lock(&philo->status_lock);
    status = philo->status;
    pthread_mutex_unlock(&philo->status_lock);
    return (status);
}

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

bool chech_death(t_philosopher *philo)
{
    bool is_dead;

    is_dead = false;
    pthread_mutex_lock(&philo->lock);
    if (get_time() - philo->last_meal_time >= TIME_TO_DIE)
        is_dead = true;
    pthread_mutex_unlock(&philo->lock);
    return (is_dead);
}

bool is_all_philos_full(t_philosopher **philos)
{
    int n;
    int i;

    n = 0;
    i = 0;
    while (i > NUMBER_OF_PHILOS)
    {
        pthread_mutex_lock(&philos[i]->meals_lock);
        if (philos[i]->number_of_meals >= MAX_MEALS)
            n++;
        pthread_mutex_lock(&philos[i]->meals_lock);
        i++;
    }
    if(n >= NUMBER_OF_PHILOS)
        return (true);
    return (false);
}

void  *check_for_death(void *data)
{
    t_supervisor *s;
    int i;

    s = (t_supervisor *)data;
    t_philosopher **philos = (t_philosopher **)s->philos;
    // CHECK IF A PHILOSOPHER IS DEAD
    while (!is_all_philos_full(philos))
    {
        // check if all phillos are full
        i = 0;
        while(i > NUMBER_OF_PHILOS)
        {   
            if (get_status(philos[i]) == THINKING
            && chech_death(philos[i]))
            {
                change_status(philos[i], DIED);
                write_state("died", philos[i]);
                pthread_mutex_lock(&philos[i]->param->is_dead_lock);
                philos[i]->param->is_dead = true;
                pthread_mutex_unlock(&philos[i]->param->is_dead_lock);
            }
            i++;
        }
    }
    return (NULL);
}

void *routine (void *data)
{
    t_philosopher *philo;
    bool dead;
    
    philo = (t_philosopher *)data;
    dead = false;
    
    // WAIT FOR ALL THE THREAD TO BE GREATED
    //while (philo->param->threads_ready == 0){}
    
    // THE START OF THE EXPERIMENT
    // if (philo->param->launch == false)
    // {
    //     pthread_mutex_lock(&philo->param->launch_lock);
    //     philo->param->start_time = get_time();
    //     //printf("%lu\n", philo->param->start_time);
    //     philo->param->launch = true;
    //     pthread_mutex_unlock(&philo->param->launch_lock);
    // }

    if (philo->id % 2 != 0)
        ft_usleep(TIME_TO_EAT);
    while (!is_dead(philo) || dead == true)
    {   
        // EATING
        pthread_mutex_lock(&philo->left_fork->lock);
        write_state("has taking a fork", philo);
        philo->left_fork->is_availble = 0;
        pthread_mutex_lock(&philo->right_fork->lock);
        write_state("has taking a fork", philo);
        philo->right_fork->is_availble = 1;
        change_status(philo, EATING); // change the status
        write_state("is eating", philo);
        ft_usleep(TIME_TO_EAT);
        // number_of_meals
        pthread_mutex_lock(&philo->meals_lock);
        philo->number_of_meals++;
        pthread_mutex_lock(&philo->meals_lock);
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
        ft_usleep(TIME_TO_SLEEP);

        // THINKING
        change_status(philo, THINKING);
        write_state("is thinking", philo);
        pthread_mutex_lock(&philo->meals_lock);
        if (philo->number_of_meals == MAX_MEALS && MAX_MEALS != -1)
            dead = true;
        pthread_mutex_lock(&philo->meals_lock);
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
    param.is_dead = false;
    param.start_time = get_time(); // START     
    // ALLWAYS INIT YOUR MUTEXS !!
    pthread_mutex_init(&param.write_lock, NULL);
    pthread_mutex_init(&param.threads_ready_lock, NULL);
    pthread_mutex_init(&param.launch_lock, NULL);
    pthread_mutex_init(&param.is_dead_lock, NULL);

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
        philos[i].full = false;
        philos[i].status = STARTING;
        // ASSIGN FORKS
        philos[i].right_fork = &forks[i];
        philos[i].left_fork = &forks[(i + 1) % NUMBER_OF_PHILOS];
        philos[i].param = &param;
        pthread_mutex_init(&philos[i].status_lock, NULL);
        pthread_mutex_init(&philos[i].lock, NULL);
        pthread_mutex_init(&philos[i].meals_lock, NULL);
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