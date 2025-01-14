/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:51:03 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 13:04:54 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	wait_thread(pthread_t *threads, int philo_count, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < philo_count)
		pthread_join(threads[i++], NULL);
	free(threads);
	if (monitor != NULL)
	{
		pthread_join(*monitor, NULL);
		free(monitor);
	}
}

static int	generate_philosophers(t_data *data)
{
	pthread_t	*threads;
	pthread_t	*monitor;
	int			i;

	threads = malloc(data->philo_count * sizeof(pthread_t));
	if (threads == NULL)
		return (EXIT_FAILURE);
	data->start_time = get_timestamp();
	i = -1;
	while (++i < data->philo_count)
	{
		if (philo_init(data, i) == 0)
		{
			set_quit_to_1(data);
			return (wait_thread(threads, i, NULL), EXIT_FAILURE);
		}
		if (pthread_create(&threads[i], NULL, philo_routine,
				&(data->philosophers[i])) != 0)
		{
			set_quit_to_1(data);
			return (wait_thread(threads, i, NULL), EXIT_FAILURE);
		}
	}
	monitor = create_monitor(data);
	return (wait_thread(threads, i, monitor), EXIT_SUCCESS);
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
