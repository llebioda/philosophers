/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:26:21 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 13:54:39 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_is_dead(t_philosopher *philo)
{
	long	last_meal_time;

	if (get_quit_value(philo->data) == 1)
		return (1);
	last_meal_time = get_mutex_value(&(philo->last_meal_time_mutex),
			&(philo->last_meal_time));
	if (get_timestamp() - last_meal_time > philo->data->time_to_die)
	{
		log_state(philo, "died");
		return (1);
	}
	return (0);
}

int	check_simulation_ended(t_data *data)
{
	int	i;

	if (get_quit_value(data) == 1)
		return (1);
	if (data->min_eat_count < 0)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		if (get_mutex_value(&(data->philosophers[i].eat_count_mutex),
				&(data->philosophers[i].eat_count)) < data->min_eat_count)
			return (0);
		i++;
	}
	set_quit_to_1(data);
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
	set_mutex_value(&(philo->last_meal_time_mutex),
		&(philo->last_meal_time), get_timestamp());
	if (philo->data->time_to_eat * 1000 > 0)
		usleep(philo->data->time_to_eat * 1000);
	set_mutex_value(&(philo->eat_count_mutex), &(philo->eat_count),
		get_mutex_value(&(philo->eat_count_mutex), &(philo->eat_count)) + 1);
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
	return (1);
}

// int	eat_philo(t_philosopher *philo)
// {
// 	long	time;
// 	long	last_meal_time;

// 	last_meal_time = get_mutex_value(&(philo->last_meal_time_mutex),
// 			&(philo->last_meal_time));
// 	time = philo->data->time_to_die - (get_timestamp() - last_meal_time);
// 	log_state(philo, "is eating");
// 	if (time <= philo->data->time_to_eat)
// 	{
// 		if (time * 1000 > 0)
// 			usleep(time * 1000);
// 		log_state(philo, "died");
// 		pthread_mutex_unlock(philo->forks[0]);
// 		return (pthread_mutex_unlock(philo->forks[1]), 0);
// 	}
// 	set_mutex_value(&(philo->last_meal_time_mutex),
// 		&(philo->last_meal_time), get_timestamp());
// 	if (philo->data->time_to_eat * 1000 > 0)
// 		usleep(philo->data->time_to_eat * 1000);
// 	philo->eat_count++;
// 	pthread_mutex_unlock(philo->forks[0]);
// 	return (pthread_mutex_unlock(philo->forks[1]), 1);
// }

int	sleep_philo(t_philosopher *philo)
{
	long	time;
	int		return_value;
	long	last_meal_time;

	last_meal_time = get_mutex_value(&(philo->last_meal_time_mutex),
			&(philo->last_meal_time));
	time = philo->data->time_to_die - (get_timestamp() - last_meal_time);
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
