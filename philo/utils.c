/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:59:36 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/20 10:41:23 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	log_state(t_philosopher *philosopher, const char *state)
{
	long	timestamp;

	if (philosopher == NULL || philosopher->data->quit == 1 || state == NULL)
		return ;
	timestamp = get_timestamp() - philosopher->data->start_time;
	pthread_mutex_lock(&(philosopher->data->print_mutex));
	if (philosopher->data->quit == 0)
	{
		printf("%ld %d %s\n", timestamp, philosopher->id, state);
		if (ft_strcmp(state, "died") == 0)
			philosopher->data->quit = 1;
	}
	pthread_mutex_unlock(&(philosopher->data->print_mutex));
}

int	parse_arg(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->philo_count = (int)ft_atol(argv[1]);
	if (data->philo_count <= 0)
		return (write(2, "The argument 'numbers_of_philo' is not valid\n",
				45), 0);
	data->time_to_die = ft_atol(argv[2]);
	if (data->time_to_die <= 0)
		return (write(2, "The argument 'time_to_die' is not valid\n", 40), 0);
	data->time_to_eat = ft_atol(argv[3]);
	if (data->time_to_eat < 0)
		return (write(2, "The argument 'time_to_eat' is not valid\n", 40), 0);
	data->time_to_sleep = ft_atol(argv[4]);
	if (data->time_to_sleep < 0)
		return (write(2, "The argument 'time_to_sleep' is not valid\n", 42), 0);
	data->min_eat_count = -1;
	if (argc == 6)
	{
		data->min_eat_count = ft_atol(argv[5]);
		if (data->min_eat_count <= 0)
			return (write(2, "The argument 'min_eat_count' is not valid\n",
					42), 0);
	}
	return (1);
}

int	init_data(t_data *data)
{
	int	i;

	data->forks = malloc(data->philo_count * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
		return (0);
	data->philosophers = malloc(data->philo_count * sizeof(t_philosopher));
	if (data->philosophers == NULL)
		return (free(data->forks), 0);
	if (pthread_mutex_init(&(data->print_mutex), NULL) != 0)
		return (free(data->forks), free(data->philosophers), 0);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL) != 0)
		{
			pthread_mutex_destroy(&(data->print_mutex));
			while (i >= 0)
				pthread_mutex_destroy(&(data->forks[i--]));
			return (free(data->forks), free(data->philosophers), 0);
		}
		i++;
	}
	return (1);
}

void	free_data(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&(data->print_mutex));
	i = 0;
	while (i < data->philo_count)
		pthread_mutex_destroy(&(data->forks[i++]));
	free(data->forks);
	free(data->philosophers);
}
