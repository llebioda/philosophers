/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:51:40 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/23 22:19:47 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_philosopher	t_philosopher;

typedef struct s_data
{
	long			start_time;
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			min_eat_count;
	int				quit;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
}	t_data;

typedef struct s_philosopher
{
	int				id;
	long			eat_count;
	pthread_mutex_t	*forks[2];
	long			last_meal_time;
	t_data			*data;
}	t_philosopher;

long	ft_atol(const char *nptr);
int		ft_strcmp(const char *s1, const char *s2);
long	get_timestamp(void);
void	log_state(t_philosopher *philosopher, const char *state);
int		parse_arg(t_data *data, int argc, char **argv);
int		init_data(t_data *data);
void	free_data(t_data *data);
int		check_is_dead(t_philosopher *philo);
int		check_simulation_ended(t_data *data);
int		get_forks(t_philosopher *philo);
int		eat_philo(t_philosopher *philo);
int		sleep_philo(t_philosopher *philo);
void	*philo_monitor(void *arg);

#endif
