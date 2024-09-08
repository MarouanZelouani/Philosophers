#include "../includes/philo.h"

int	ft_isdigit(int arg)
{
	if (arg >= '0' && arg <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int					sign;
	unsigned long		result;
	size_t					i;

	sign = 1;
	result = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\n' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + str[i] - '0';
		i++;
	}
	return ((int)sign * result);
}

int ft_strcmp(char *s1, char *s2)
{
    size_t	i;
	
	i = 0;
    while (s1[i] || s2[i])
    {
        if (s1[i] - s2[i] != 0)
            return (s1[i] - s2[i]);
    }
    return 0;
}
