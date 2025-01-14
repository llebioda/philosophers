/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:15:56 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 11:21:56 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (check_is_dead(&(data->philosophers[i])) == 1)
			{
				set_quit_to_1(data);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}

pthread_t	*create_monitor(t_data *data)
{
	pthread_t	*monitor;

	monitor = malloc(sizeof(pthread_t));
	if (monitor == NULL)
		return (NULL);
	if (pthread_create(monitor, NULL, philo_monitor, data) != 0)
	{
		set_quit_to_1(data);
		return (NULL);
	}
	return (monitor);
}
