/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:51:03 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/23 22:21:31 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_start(t_philosopher *philo)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, philo_monitor, philo) != 0)
	{
		philo->data->quit = 1;
		return ;
	}
	pthread_detach(monitor);
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

static void	*philo_routine(void *arg)
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

static int	generate_philosophers(t_data *data)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(data->philo_count * sizeof(pthread_t));
	if (threads == NULL)
		return (EXIT_FAILURE);
	data->start_time = get_timestamp();
	i = 0;
	while (i < data->philo_count)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].eat_count = 0;
		data->philosophers[i].last_meal_time = get_timestamp();
		data->philosophers[i].data = data;
		if (pthread_create(&threads[i], NULL, philo_routine,
				&(data->philosophers[i])) != 0)
			return (free(threads), data->quit = 1, EXIT_FAILURE);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
		pthread_join(threads[i++], NULL);
	free(threads);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		return_value;

	if (argc != 5 && argc != 6)
	{
		write(2, "Usage: ./philo numbers_of_philo time_to_die", 43);
		write(2, " time_to_eat time_to_sleep [min_eat_count]\n", 43);
		return (EXIT_FAILURE);
	}
	if (parse_arg(&data, argc, argv) == 0)
		return (EXIT_FAILURE);
	if (init_data(&data) == 0)
		return (EXIT_FAILURE);
	return_value = generate_philosophers(&data);
	free_data(&data);
	return (return_value);
}
