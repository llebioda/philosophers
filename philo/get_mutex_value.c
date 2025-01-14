/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_mutex_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:10:18 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 11:53:15 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_quit_value(t_data *data)
{
	int	value;

	pthread_mutex_lock(&(data->quitmutex));
	value = data->quit;
	pthread_mutex_unlock(&(data->quitmutex));
	return (value);
}

void	set_quit_to_1(t_data *data)
{
	pthread_mutex_lock(&(data->quitmutex));
	data->quit = 1;
	pthread_mutex_unlock(&(data->quitmutex));
}

long	get_mutex_value(pthread_mutex_t *mutex, long *ptr)
{
	long	value;

	pthread_mutex_lock(mutex);
	value = *ptr;
	pthread_mutex_unlock(mutex);
	return (value);
}

void	set_mutex_value(pthread_mutex_t *mutex, long *ptr, long new_value)
{
	pthread_mutex_lock(mutex);
	*ptr = new_value;
	pthread_mutex_unlock(mutex);
}
