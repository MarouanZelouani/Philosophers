/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:09 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/10 04:35:50 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main (int ac, char **av)
{
    t_philosopher *philos;
    t_monitor *monitor;
    t_param param;
    t_fork *forks;

    philos = NULL;
    forks = NULL;
    monitor = NULL;
    if (get_data(ac, av, &param))
        return (EXIT_FAILURE);
    if (__init(&philos, &forks, &param, &monitor))
      return (EXIT_FAILURE);
    if (1 == param.number_of_philosophers)
        return (handle_one_philo(philos));
    if (start_sumulation(philos, monitor))
        return (EXIT_FAILURE);
    if (threads_join(philos, monitor))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

