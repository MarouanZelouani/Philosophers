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
    if (philo->status != DIED)
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

// CHECK IF ALL PHILOSOPHERS ARE FULL
bool dinner_end(t_philosopher *philos)
{
    int n;
    int i;

    n = 0;
    i = 0;
    // printf("check started\n");
    while (i < NUMBER_OF_PHILOS)
    {
        //write_state("checking...", &philos[i]);
        pthread_mutex_lock(&philos[i].meals_lock);
        if (philos[i].number_of_meals >= MAX_MEALS
        && MAX_MEALS != -1)
            n++;
        pthread_mutex_unlock(&philos[i].meals_lock);
        i++;
    }
    // if (is_dead(&philos[0]) == true)
    //         printf("dead\n");
    if(n >= NUMBER_OF_PHILOS || is_dead(&philos[0]))
        return (true);
    return (false);
}

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] || s2[i])
    {
        if (s1[i] - s2[i] != 0)
            return (s1[i] - s2[i]);
    }
    return 0;
}

void write_state(char *s, t_philosopher *philo)
{
    pthread_mutex_lock(&philo->param->write_lock);
    if (philo->last_msg == false)
        printf("%lu %d %s\n", get_time() - philo->param->start_time, philo->id, s);
    pthread_mutex_unlock(&philo->param->write_lock);
}

// CHECK IF A PHILOSOPHER DIED
void  *check_for_death(void *data)
{
    t_supervisor *s;
    int i;

    s = (t_supervisor *)data;
    //t_philosopher *philos = (t_philosopher *)s->philos;
    // // CHECK IF A PHILOSOPHER IS DEAD
    // while (!is_all_philos_full(philos))
    // {
    //     // check if all phillos are full
    //     i = 0;
    //     while(i > NUMBER_OF_PHILOS)
    //     {   
    //         if (get_status(philos[i]) == THINKING
    //         && chech_death(philos[i]))
    //         {
    //             change_status(philos[i], DIED);
    //             write_state("died", philos[i]);
    //             pthread_mutex_lock(&philos[i]->param->is_dead_lock);
    //             philos[i]->param->is_dead = true;
    //             pthread_mutex_unlock(&philos[i]->param->is_dead_lock);
    //         }
    //         i++;
    //     }
    // }

    // printf("supervisor start\n");
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
    // printf("supervisor finish\n");
    return (NULL);
}

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
    // if (philo->id % 2 != 0)
    //     ft_usleep(TIME_TO_EAT);
    while (!dead && is_dead(philo) == false)
    {   
        // EATING
        // pthread_mutex_lock(&philo->left_fork->lock);
        // write_state("has taking a fork", philo);
        // philo->left_fork->is_availble = 0;
        // pthread_mutex_lock(&philo->right_fork->lock);
        // // if (get_status(philo) == DIED)
        // //     break;
        // write_state("has taking a fork", philo);
        // philo->right_fork->is_availble = 1;
        take_forks(philo);
        change_status(philo, EATING); // change the status
        write_state("is eating", philo);
        ft_usleep(TIME_TO_EAT);
        
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
        ft_usleep(TIME_TO_SLEEP);

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
        // printf("nn\n");
        philos[i].id = i + 1;
        philos[i].number_of_meals = 0;
        philos[i].full = false;
        philos[i].last_msg = false;
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
    supervisor.philos = philos;
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