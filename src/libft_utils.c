/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzelouan <mzelouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:21:06 by mzelouan          #+#    #+#             */
/*   Updated: 2024/11/12 02:16:51 by mzelouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_isdigit(int arg)
{
	if (arg >= '0' && arg <= '9')
		return (1);
	return (0);
}

size_t	ft_atoi(const char *str)
{
	size_t	result;
	size_t	i;

	i = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + str[i] - '0';
		i++;
	}
	return (result);
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

char	*ft_itoa(size_t number)
{
	int		count;
	char	*s;
	int		n;

	count = 1;
	n = number;
	while (n / 10 != 0)
	{
		n /= 10;
		count++;
	}
	s = (char *)malloc((count + 1) * sizeof(char));
	if (s == NULL)
		return (NULL);
	s[count] = '\0';
	while (count > 0)
	{
		s[count - 1] = (number % 10) + 48;
		number /= 10;
		count--;
	}
	return (s);
}
