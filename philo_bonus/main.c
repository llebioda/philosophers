/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:51:03 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 13:23:33 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	wait_child(t_data *data, pid_t *pids)
{
	int	i;
	int	status;

	if (pthread_create(data->done_monitor, NULL, &done_monitoring, data) != 0)
	{
		i = 0;
		while (i < data->philo_count)
			kill(pids[i++], SIGTERM);
	}
	else
	{
		data->done_monitor_initialized = 1;
		waitpid(-1, &status, 0);
		if (!WIFEXITED(status) || WEXITSTATUS(status) == EXIT_FAILURE)
		{
			i = 0;
			while (i < data->philo_count)
				kill(pids[i++], SIGTERM);
		}
	}
}

static void	philo_start(t_philosopher *philo)
{
	log_state(philo, "is thinking");
	if (philo->data.philo_count % 2 == 1)
	{
		if (philo->id == 1)
			return ;
		else if (philo->id % 2 == 0)
			usleep(philo->data.time_to_eat * 500);
		else
			usleep(philo->data.time_to_eat * 1000);
	}
	else if (philo->id % 2 == 1)
		usleep(philo->data.time_to_eat * 1000);
}

static void	init_philosophers(t_philosopher philo, int id)
{
	pthread_t	philo_monitor;

	philo.id = id;
	philo.eat_count = 0;
	philo.has_reached_eat_count = 0;
	philo.last_meal_time = get_timestamp();
	philo.quit = 0;
	if (pthread_mutex_init(&(philo.quit_mutex), NULL) != 0)
	{
		free_data(&(philo.data), 0);
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&philo_monitor, NULL, &philo_monitoring, &philo) != 0)
	{
		pthread_mutex_destroy(&(philo.quit_mutex));
		free_data(&(philo.data), 0);
		exit(EXIT_FAILURE);
	}
	philo_start(&philo);
	philo_routine(&philo);
	sem_post(philo.data.done_sem);
	pthread_join(philo_monitor, NULL);
	pthread_mutex_destroy(&(philo.quit_mutex));
	free_data(&(philo.data), 0);
	exit(EXIT_SUCCESS);
}

static int	generate_philosophers(t_data *data)
{
	pid_t			*pids;
	t_philosopher	philosopher;
	int				i;

	pids = malloc(data->philo_count * sizeof(pid_t));
	if (pids == NULL)
		return (EXIT_FAILURE);
	i = 0;
	data->start_time = get_timestamp();
	philosopher.data = *data;
	while (i < data->philo_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			while (--i >= 0)
				kill(pids[i], SIGTERM);
			return (free(pids), EXIT_FAILURE);
		}
		if (pids[i++] == 0)
			return (free(pids), init_philosophers(philosopher, i), 0);
	}
	wait_child(data, pids);
	return (free(pids), EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	done_monitor;
	int			exit_code;

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
	if (pthread_mutex_init(&(data.quit_mutex), NULL) != 0)
	{
		free_data(&data, 1);
		return (EXIT_FAILURE);
	}
	data.quit_mutex_init = 1;
	data.done_monitor = &done_monitor;
	exit_code = generate_philosophers(&data);
	free_data(&data, 1);
	return (exit_code);
}
