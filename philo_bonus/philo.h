/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:51:40 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 13:24:03 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <signal.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>

typedef struct s_philosopher	t_philosopher;

typedef struct s_data
{
	long			start_time;
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			min_eat_count;
	sem_t			*forks;
	sem_t			*access_fork;
	sem_t			*print_sem;
	sem_t			*done_sem;
	sem_t			*quit_sem;
	int				quit;
	char			quit_mutex_init;
	pthread_mutex_t	quit_mutex;
	pthread_t		*done_monitor;
	int				done_monitor_initialized;
}	t_data;

typedef struct s_philosopher
{
	int				id;
	long			eat_count;
	int				has_reached_eat_count;
	long			last_meal_time;
	t_data			data;
	int				quit;
	pthread_mutex_t	quit_mutex;
}	t_philosopher;

long	ft_atol_safe(const char *nptr, long error_ret_val);
int		ft_strcmp(const char *s1, const char *s2);

long	get_timestamp(void);
void	log_state(t_philosopher *philosopher, const char *state);
int		parse_arg(t_data *data, int argc, char **argv);
int		init_data(t_data *data);
void	free_data(t_data *data, int unlink_sem);

void	*done_monitoring(void *arg);
void	philo_routine(t_philosopher	*philo);
void	*philo_monitoring(void *arg);

int		get_mutex_value(pthread_mutex_t *mutex, int *ptr);
void	set_mutex_value(pthread_mutex_t *mutex, int *ptr, int new_value);

#endif
