/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:34:13 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 13:14:34 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*done_monitoring(void *arg)
{
	t_data	*data;
	int		done_count;

	data = (t_data *)arg;
	done_count = 0;
	while (done_count < data->philo_count
		&& get_mutex_value(&(data->quit_mutex), &(data->quit)) == 0)
	{
		sem_wait(data->done_sem);
		done_count++;
	}
	sem_post(data->quit_sem);
	return (NULL);
}

void	*philo_monitoring(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	sem_wait(philo->data.quit_sem);
	set_mutex_value(&(philo->quit_mutex), &(philo->quit), 1);
	sem_post(philo->data.quit_sem);
	return (NULL);
}

int	get_mutex_value(pthread_mutex_t *mutex, int *ptr)
{
	int	value;

	pthread_mutex_lock(mutex);
	value = *ptr;
	pthread_mutex_unlock(mutex);
	return (value);
}

void	set_mutex_value(pthread_mutex_t *mutex, int *ptr, int new_value)
{
	pthread_mutex_lock(mutex);
	*ptr = new_value;
	pthread_mutex_unlock(mutex);
}
