/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:13 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 03:06:24 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void		_error(int code, char **av);
int			check_arguments(char **av);
static int	check_numbers(t_args *args);
size_t		parse_arg(char *arg);

int	get_data(int ac, char **av, t_args *args)
{
	int	error;

	if (ac != 5 && ac != 6)
		_error(0, av);
	error = check_arguments(av);
	if (error != 0)
		_error(error, av);
	args->number_of_philosophers = parse_arg(av[1]);
	args->time_to_die = parse_arg(av[2]);
	args->time_to_eat = parse_arg(av[3]);
	args->time_to_sleep = parse_arg(av[4]);
	args->number_of_meals = -1;
	if (ac == 6)
		args->number_of_meals = ft_atoi(av[5]);
	if (check_numbers(args))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

size_t	parse_arg(char *arg)
{
	size_t	result;
	char	*tmp_res;

	result = ft_atoi(arg);
	tmp_res = ft_itoa(result);
	if (arg && arg[0] && arg[0] == '+')
		arg++;
	if (ft_strcmp(tmp_res, arg))
	{
		printf("Error: The provided number '%s' is out of range!\n", arg);
		free(tmp_res);
		exit(EXIT_FAILURE);
	}
	free(tmp_res);
	return (result);
}

void	_error(int code, char **av)
{
	if (code == 0)
	{
		printf("Usage : ./philo [1] [2] [3] [4] [optional]\n");
		printf("\t [1] - number of philosophers\n");
		printf("\t [2] - time to die\n");
		printf("\t [3] - time to eat\n");
		printf("\t [4] - time to sleep\n");
		printf("\t [optional] : number of times each philosopher must_eat\n");
	}
	else if (code > 0)
		printf("Error: '%s' is not a valid argument!!\n", av[code]);
	exit(EXIT_FAILURE);
}

int	check_arguments(char **av)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		if (av[i][j] && av[i][j] == '+' && av[i][j + 1])
			j++;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return (i);
			j++;
		}
		i++;
	}
	return (0);
}

static int	check_numbers(t_args *args)
{
	if (args == NULL)
		return (EXIT_FAILURE);
	if (args->number_of_philosophers <= 0)
		return (EXIT_FAILURE);
	if (args->number_of_meals == 0 || args->number_of_meals < -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
