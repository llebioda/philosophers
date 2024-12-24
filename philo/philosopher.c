/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:26:21 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/23 22:24:12 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_is_dead(t_philosopher *philo)
{
	if (philo->data->quit == 1)
		return (1);
	if (get_timestamp() - philo->last_meal_time > philo->data->time_to_die)
	{
		log_state(philo, "died");
		return (1);
	}
	return (0);
}

int	check_simulation_ended(t_data *data)
{
	int	i;

	if (data->quit == 1)
		return (1);
	if (data->min_eat_count < 0)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		if (data->philosophers[i].eat_count < data->min_eat_count)
			return (0);
		i++;
	}
	data->quit = 1;
	return (1);
}

int	get_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->forks[philo->id % 2]);
	if (check_is_dead(philo) == 1)
		return (pthread_mutex_unlock(philo->forks[philo->id % 2]), 0);
	log_state(philo, "has taken a fork");
	if (philo->data->philo_count == 1)
	{
		if (philo->data->time_to_die * 1000 > 0)
			usleep(philo->data->time_to_die * 1000);
		log_state(philo, "died");
		pthread_mutex_unlock(philo->forks[philo->id % 2]);
		return (0);
	}
	pthread_mutex_lock(philo->forks[(philo->id + 1) % 2]);
	if (check_is_dead(philo) == 1)
	{
		pthread_mutex_unlock(philo->forks[0]);
		pthread_mutex_unlock(philo->forks[1]);
		return (0);
	}
	log_state(philo, "has taken a fork");
	return (1);
}

int	eat_philo(t_philosopher *philo)
{
	log_state(philo, "is eating");
	if (philo->data->time_to_eat * 1000 > 0)
		usleep(philo->data->time_to_eat * 1000);
	philo->last_meal_time = get_timestamp();
	philo->eat_count++;
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
	return (1);
}

// int	eat_philo(t_philosopher *philo)
// {
// 	long	time;
// 	int		return_value;

// 	time = philo->data->time_to_die - (get_timestamp() - philo->last_meal_time);
// 	return_value = 0;
// 	log_state(philo, "is eating");
// 	if (time <= philo->data->time_to_eat)
// 	{
// 		if (time * 1000 > 0)
// 			usleep(time * 1000);
// 		log_state(philo, "died");
// 	}
// 	else
// 	{
// 		if (philo->data->time_to_eat * 1000 > 0)
// 			usleep(philo->data->time_to_eat * 1000);
// 		philo->last_meal_time = get_timestamp();
// 		philo->eat_count++;
// 		return_value = 1;
// 	}
// 	pthread_mutex_unlock(philo->forks[0]);
// 	pthread_mutex_unlock(philo->forks[1]);
// 	return (return_value);
// }

int	sleep_philo(t_philosopher *philo)
{
	long	time;
	int		return_value;

	time = philo->data->time_to_die - (get_timestamp() - philo->last_meal_time);
	return_value = 0;
	log_state(philo, "is sleeping");
	if (time <= philo->data->time_to_sleep)
	{
		if (time * 1000 > 0)
			usleep(time * 1000);
		log_state(philo, "died");
	}
	else
	{
		if (philo->data->time_to_sleep * 1000 > 0)
			usleep(philo->data->time_to_sleep * 1000);
		return_value = 1;
	}
	return (return_value);
}
