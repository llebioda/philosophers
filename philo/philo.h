/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:51:40 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 12:54:20 by llebioda         ###   ########.fr       */
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
	pthread_mutex_t	quitmutex;
}	t_data;

typedef struct s_philosopher
{
	int				id;
	long			eat_count;
	long			last_meal_time;
	pthread_mutex_t	*forks[2];
	pthread_mutex_t	eat_count_mutex;
	char			eat_count_mutex_init;
	pthread_mutex_t	last_meal_time_mutex;
	char			last_meal_time_mutex_init;
	t_data			*data;
}	t_philosopher;

void		*ft_calloc(size_t nmemb, size_t size);
long		ft_atol_safe(const char *nptr, long error_ret_val);
int			ft_strcmp(const char *s1, const char *s2);

long		get_timestamp(void);
void		log_state(t_philosopher *philosopher, const char *state);
int			parse_arg(t_data *data, int argc, char **argv);
int			init_data(t_data *data);
void		free_data(t_data *data);

void		*philo_routine(void *arg);
int			philo_init(t_data *data, int id);

int			check_is_dead(t_philosopher *philo);
int			check_simulation_ended(t_data *data);
int			get_forks(t_philosopher *philo);
int			eat_philo(t_philosopher *philo);
int			sleep_philo(t_philosopher *philo);

pthread_t	*create_monitor(t_data *data);
int			get_quit_value(t_data *data);
void		set_quit_to_1(t_data *data);
long		get_mutex_value(pthread_mutex_t *mutex, long *ptr);
void		set_mutex_value(pthread_mutex_t *mutex, long *ptr, long new_value);
#endif
