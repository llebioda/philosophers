/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:13 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 11:57:53 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_start(t_philosopher *philo)
{
	log_state(philo, "is thinking");
	if (philo->data->philo_count % 2 == 1)
	{
		if (philo->id == 1)
			return ;
		else if (philo->id % 2 == 0)
			usleep(philo->data->time_to_eat * 500);
		else
			usleep(philo->data->time_to_eat * 1000);
	}
	else if (philo->id % 2 == 1)
		usleep(philo->data->time_to_eat * 1000);
}

void	*philo_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	p->forks[0] = &(p->data->forks[p->id - 1]);
	p->forks[1] = &(p->data->forks[p->id % p->data->philo_count]);
	philo_start(p);
	while (1)
	{
		if (get_forks(p) == 0)
			return (NULL);
		if (check_is_dead(p) == 1)
			return (pthread_mutex_unlock(p->forks[0]),
				pthread_mutex_unlock(p->forks[1]), NULL);
		if (eat_philo(p) == 0)
			return (NULL);
		if (check_simulation_ended(p->data) == 1)
			return (NULL);
		if (check_is_dead(p) == 1)
			return (NULL);
		if (sleep_philo(p) == 0)
			return (NULL);
		if (check_is_dead(p) == 1)
			return (NULL);
		log_state(p, "is thinking");
	}
}

int	philo_init(t_data *data, int id)
{
	t_philosopher	*philo;

	philo = &(data->philosophers[id]);
	if (pthread_mutex_init(&(philo->eat_count_mutex), NULL) != 0)
		return (0);
	philo->eat_count_mutex_init = 1;
	if (pthread_mutex_init(&(philo->last_meal_time_mutex), NULL) != 0)
		return (pthread_mutex_destroy(&(philo->eat_count_mutex)), 0);
	philo->last_meal_time_mutex_init = 1;
	philo->id = id + 1;
	philo->eat_count = 0;
	philo->last_meal_time = get_timestamp();
	philo->data = data;
	return (1);
}
