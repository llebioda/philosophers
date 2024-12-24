/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:34:13 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/23 22:22:45 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*done_monitoring(void *arg)
{
	t_data	*data;
	int		done_count;

	data = (t_data *)arg;
	done_count = 0;
	while (data->quit == 0 && done_count < data->philo_count)
	{
		sem_wait(data->done_sem);
		done_count++;
	}
	sem_post(data->quit_sem);
	return (NULL);
}

void	*quit_monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	sem_wait(data->quit_sem);
	data->quit = 1;
	sem_post(data->quit_sem);
	sem_post(data->done_sem);
	return (NULL);
}

void	*philo_monitoring(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	sem_wait(philo->data.quit_sem);
	philo->quit = 1;
	sem_post(philo->data.quit_sem);
	return (NULL);
}
