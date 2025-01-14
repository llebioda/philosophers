/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:59:36 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 13:30:17 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	memset(&tv, 0, sizeof(struct timeval));
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	log_state(t_philosopher *philo, const char *state)
{
	long	timestamp;

	if (philo == NULL
		|| get_mutex_value(&(philo->quit_mutex), &(philo->quit)) == 1)
		return ;
	timestamp = get_timestamp() - philo->data.start_time;
	sem_wait(philo->data.print_sem);
	pthread_mutex_lock(&(philo->quit_mutex));
	if (philo->quit == 0)
	{
		printf("%ld %d %s\n", timestamp, philo->id, state);
		if (ft_strcmp(state, "died") == 0)
		{
			philo->quit = 1;
			sem_post(philo->data.quit_sem);
		}
	}
	pthread_mutex_unlock(&(philo->quit_mutex));
	sem_post(philo->data.print_sem);
}

int	parse_arg(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->philo_count = (int)ft_atol_safe(argv[1], -1);
	if (data->philo_count <= 0)
		return (write(2, "The argument 'numbers_of_philo' is not valid\n",
				45), 0);
	data->time_to_die = ft_atol_safe(argv[2], -1);
	if (data->time_to_die <= 0)
		return (write(2, "The argument 'time_to_die' is not valid\n", 40), 0);
	data->time_to_eat = ft_atol_safe(argv[3], -1);
	if (data->time_to_eat < 0)
		return (write(2, "The argument 'time_to_eat' is not valid\n", 40), 0);
	data->time_to_sleep = ft_atol_safe(argv[4], -1);
	if (data->time_to_sleep < 0)
		return (write(2, "The argument 'time_to_sleep' is not valid\n", 42), 0);
	data->min_eat_count = -1;
	if (argc == 6)
	{
		data->min_eat_count = ft_atol_safe(argv[5], -1);
		if (data->min_eat_count <= 0)
			return (write(2, "The argument 'min_eat_count' is not valid\n",
					42), 0);
	}
	return (1);
}

int	init_data(t_data *data)
{
	sem_unlink("/forks");
	sem_unlink("/access_fork");
	sem_unlink("/print_sem");
	sem_unlink("/done_sem");
	sem_unlink("/quit_sem");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->philo_count);
	if (data->forks == SEM_FAILED)
		return (0);
	data->access_fork = sem_open("/access_fork", O_CREAT, 0644, 1);
	if (data->access_fork == SEM_FAILED)
		return (sem_close(data->forks), sem_unlink("/forks"), 0);
	data->done_sem = sem_open("/done_sem", O_CREAT, 0644, 0);
	if (data->done_sem == SEM_FAILED)
		return (sem_close(data->forks), sem_unlink("/forks"),
			sem_close(data->access_fork), sem_unlink("/access_fork"), 0);
	data->quit_sem = sem_open("/quit_sem", O_CREAT, 0644, 0);
	if (data->quit_sem == SEM_FAILED)
		return (sem_close(data->forks), sem_unlink("/forks"),
			sem_close(data->access_fork), sem_unlink("/access_fork"),
			sem_close(data->done_sem), sem_unlink("/done_sem"), 0);
	data->print_sem = sem_open("/print_sem", O_CREAT, 0644, 1);
	if (data->print_sem == SEM_FAILED)
		return (free_data(data, 1), 0);
	return (1);
}

void	free_data(t_data *data, int unlink_sem)
{
	if (data->quit_mutex_init == 1)
		set_mutex_value(&(data->quit_mutex), &(data->quit), 1);
	else
		data->quit = 1;
	sem_post(data->done_sem);
	sem_post(data->quit_sem);
	if (data->done_monitor != NULL && data->done_monitor_initialized)
		pthread_join(*(data->done_monitor), NULL);
	if (data->quit_mutex_init == 1)
		pthread_mutex_destroy(&(data->quit_mutex));
	sem_close(data->forks);
	sem_close(data->access_fork);
	sem_close(data->done_sem);
	if (data->quit_sem != NULL)
		sem_close(data->quit_sem);
	if (data->print_sem != NULL)
		sem_close(data->print_sem);
	if (unlink_sem == 1)
	{
		sem_unlink("/forks");
		sem_unlink("/access_fork");
		sem_unlink("/print_sem");
		sem_unlink("/done_sem");
		sem_unlink("/quit_sem");
	}
}
