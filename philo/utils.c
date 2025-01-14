/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:59:36 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 12:51:54 by llebioda         ###   ########.fr       */
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

	if (philosopher == NULL || get_quit_value(philosopher->data) == 1)
		return ;
	timestamp = get_timestamp() - philosopher->data->start_time;
	pthread_mutex_lock(&(philosopher->data->print_mutex));
	pthread_mutex_lock(&(philosopher->data->quitmutex));
	if (philosopher->data->quit == 0)
	{
		printf("%ld %d %s\n", timestamp, philosopher->id, state);
		if (ft_strcmp(state, "died") == 0)
			philosopher->data->quit = 1;
	}
	pthread_mutex_unlock(&(philosopher->data->quitmutex));
	pthread_mutex_unlock(&(philosopher->data->print_mutex));
}

int	parse_arg(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->philo_count = (int)ft_atol_safe(argv[1], -1);
	if (data->philo_count <= 0)
		return (write(2, "The argument 'numbers_of_philo' is not valid\n",
				45), 0);
	data->time_to_die = ft_atol_safe(argv[2], -1);
	if (data->time_to_die <= 0)
		return (write(2, "The argument 'time_to_die' is not valid\n", 40), 0);
	data->time_to_eat = ft_atol_safe(argv[3], -1);
	if (data->time_to_eat < 0)
		return (write(2, "The argument 'time_to_eat' is not valid\n", 40), 0);
	data->time_to_sleep = ft_atol_safe(argv[4], -1);
	if (data->time_to_sleep < 0)
		return (write(2, "The argument 'time_to_sleep' is not valid\n", 42), 0);
	data->min_eat_count = -1;
	if (argc == 6)
	{
		data->min_eat_count = ft_atol_safe(argv[5], -1);
		if (data->min_eat_count <= 0)
			return (write(2, "The argument 'min_eat_count' is not valid\n",
					42), 0);
	}
	return (1);
}

int	init_data(t_data *data)
{
	int	i;

	data->forks = ft_calloc(data->philo_count, sizeof(pthread_mutex_t));
	if (data->forks == NULL)
		return (0);
	data->philosophers = ft_calloc(data->philo_count, sizeof(t_philosopher));
	if (data->philosophers == NULL)
		return (free(data->forks), 0);
	if (pthread_mutex_init(&(data->print_mutex), NULL) != 0)
		return (free(data->forks), free(data->philosophers), 0);
	if (pthread_mutex_init(&(data->quitmutex), NULL) != 0)
		return (free(data->forks), free(data->philosophers),
			pthread_mutex_destroy(&(data->print_mutex)), 0);
	i = -1;
	while (++i < data->philo_count)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL) != 0)
			break ;
	}
	if (i == data->philo_count)
		return (1);
	while (i >= 0)
		pthread_mutex_destroy(&(data->forks[i--]));
	return (free(data->philosophers), pthread_mutex_destroy(&(data->quitmutex)),
		free(data->forks), pthread_mutex_destroy(&(data->print_mutex)), 0);
}

void	free_data(t_data *data)
{
	t_philosopher	*philo;
	int				i;

	pthread_mutex_destroy(&(data->print_mutex));
	pthread_mutex_destroy(&(data->quitmutex));
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&(data->forks[i]));
		if (data->philosophers != NULL)
		{
			philo = &(data->philosophers[i]);
			if (philo->eat_count_mutex_init == 1)
				pthread_mutex_destroy(&(philo->eat_count_mutex));
			if (philo->last_meal_time_mutex_init == 1)
				pthread_mutex_destroy(&(philo->last_meal_time_mutex));
		}
		i++;
	}
	free(data->forks);
	free(data->philosophers);
}
