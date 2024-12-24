/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:26:21 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/23 22:24:24 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_is_dead(t_philosopher *philo)
{
	if (philo->quit == 1)
		return (1);
	if (get_timestamp() - philo->last_meal_time > philo->data.time_to_die)
	{
		log_state(philo, "died");
		return (1);
	}
	return (0);
}

static int	get_forks(t_philosopher *philo)
{
	sem_wait(philo->data.access_fork);
	if (check_is_dead(philo) == 1)
		return (sem_post(philo->data.access_fork), 0);
	sem_wait(philo->data.forks);
	if (check_is_dead(philo) == 1)
		return (sem_post(philo->data.access_fork),
			sem_post(philo->data.forks), 0);
	log_state(philo, "has taken a fork");
	if (philo->data.philo_count == 1)
	{
		if (philo->data.time_to_die * 1000 > 0)
			usleep(philo->data.time_to_die * 1000);
		log_state(philo, "died");
		sem_post(philo->data.access_fork);
		return (sem_post(philo->data.forks), 0);
	}
	sem_wait(philo->data.forks);
	sem_post(philo->data.access_fork);
	if (check_is_dead(philo) == 1)
		return (sem_post(philo->data.forks), sem_post(philo->data.forks), 0);
	log_state(philo, "has taken a fork");
	return (1);
}

static int	eat_philo(t_philosopher *philo)
{
	log_state(philo, "is eating");
	if (philo->data.time_to_eat * 1000 > 0)
		usleep(philo->data.time_to_eat * 1000);
	philo->last_meal_time = get_timestamp();
	philo->eat_count++;
	sem_post(philo->data.forks);
	sem_post(philo->data.forks);
	if (philo->has_reached_eat_count == 0 && philo->data.min_eat_count > 0
		&& philo->eat_count >= philo->data.min_eat_count)
	{
		philo->has_reached_eat_count = 1;
		sem_post(philo->data.done_sem);
	}
	return (1);
}

// static int	eat_philo(t_philosopher *philo)
// {
// 	long	time;
// 	int		ret_val;

// 	time = philo->data.time_to_die - (get_timestamp() - philo->last_meal_time);
// 	ret_val = 0;
// 	log_state(philo, "is eating");
// 	if (time <= philo->data.time_to_eat)
// 	{
// 		if (time * 1000 > 0)
// 			usleep(time * 1000);
// 		log_state(philo, "died");
// 	}
// 	else
// 	{
// 		if (philo->data.time_to_eat * 1000 > 0)
// 			usleep(philo->data.time_to_eat * 1000);
// 		philo->last_meal_time = get_timestamp();
// 		philo->eat_count++;
// 		ret_val = 1;
// 	}
// 	if (philo->has_reached_eat_count == 0 && philo->data.min_eat_count > 0
// 		&& philo->eat_count >= philo->data.min_eat_count)
// 		philo->has_reached_eat_count = sem_post(philo->data.done_sem) + 1;
// 	return (sem_post(philo->data.forks), sem_post(philo->data.forks), ret_val);
// }

static int	sleep_philo(t_philosopher *philo)
{
	long	time;
	int		return_value;

	time = philo->data.time_to_die - (get_timestamp() - philo->last_meal_time);
	return_value = 0;
	log_state(philo, "is sleeping");
	if (time <= philo->data.time_to_sleep)
	{
		if (time * 1000 > 0)
			usleep(time * 1000);
		log_state(philo, "died");
	}
	else
	{
		if (philo->data.time_to_sleep * 1000 > 0)
			usleep(philo->data.time_to_sleep * 1000);
		return_value = 1;
	}
	return (return_value);
}

void	philo_routine(t_philosopher	*philo)
{
	while (1)
	{
		if (get_forks(philo) == 0)
			return ;
		if (check_is_dead(philo) == 1)
		{
			sem_post(philo->data.forks);
			sem_post(philo->data.forks);
			return ;
		}
		if (eat_philo(philo) == 0)
			return ;
		if (check_is_dead(philo) == 1)
			return ;
		if (sleep_philo(philo) == 0)
			return ;
		if (check_is_dead(philo) == 1)
			return ;
		log_state(philo, "is thinking");
	}
}
