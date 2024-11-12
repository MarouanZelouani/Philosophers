/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:09 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 02:17:29 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int ac, char **av)
{
	t_philo		*philos;
	t_monitor	*monitor;
	t_args		args;
	t_fork		*forks;

	philos = NULL;
	forks = NULL;
	monitor = NULL;
	if (get_data(ac, av, &args))
		return (EXIT_FAILURE);
	if (__init(&philos, &forks, &args, &monitor))
		return (EXIT_FAILURE);
	start_sumulation(philos, monitor);
	cleanup(philos, monitor, &args, forks);
}
