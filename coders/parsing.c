/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyahyaou <nyahyaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 22:35:26 by nyahyaou          #+#    #+#             */
/*   Updated: 2026/02/26 22:41:55 by nyahyaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
}

int	check_max(int result, int *error, int digit)
{
	if (result > (INT_MAX - digit) / 10)
	{
		*error = 1;
		return (1);
	}
	return (0);
}

static int	my_atoi(char *str, int *error)
{
	long	result;
	int		i;
	int		digit;

	result = 0;
	i = 0;
	*error = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			*error = 1;
			return (0);
		}
		digit = str[i] - '0';
		if (check_max(result, error, digit))
			return (0);
		result = result * 10 + digit;
		i++;
	}
	if (result <= 0)
		*error = 1;
	return ((int)result);
}

static int	valid_argument(char *str, char *name, int i)
{
	int	error;
	int	value;

	if (i)
	{
		value = my_atoi(str, &error);
		if (error || value <= 0)
		{
			printf("Error: %s must be a positive integer\n", name);
			exit(1);
		}
		return (value);
	}
	else
	{
		lower(str);
		if (strcmp(str, "fifo") == 0)
			return (0);
		else if (strcmp(str, "edf") == 0)
			return (1);
		printf("Invalid scheduler, must be fifo or edf\n");
		exit(1);
	}
}

void	parser(char **av, t_data *data)
{
	data->number_of_coders = valid_argument(av[1], "number_of_coders", 1);
	if (data->number_of_coders == 1)
	{
		printf("can't compaling\n");
		exit(1);
	}
	data->time_to_burnout = valid_argument(av[2], "time_to_burnout", 1);
	data->time_to_compile = valid_argument(av[3], "time_to_compile", 1);
	data->time_to_debug = valid_argument(av[4], "time_to_debug", 1);
	data->time_to_refactor = valid_argument(av[5], "time_to_refactor", 1);
	data->number_of_compiles_required = valid_argument(av[6],
			"number_of_compiles_required", 1);
	data->dongle_cooldown = valid_argument(av[7], "dongle_cooldown", 1);
	data->scheduler = valid_argument(av[8], "scheduler", 0);
}
