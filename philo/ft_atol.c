/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:35:11 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/02 10:36:33 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (*s1 && *s1 == *s2 && n > 1)
	{
		s1++;
		s2++;
		n--;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

long	ft_atol(const char *nptr)
{
	long	a;
	int		is_negative;

	if (nptr == NULL)
		return (0);
	while (*nptr == ' ' || *nptr == '\f' || *nptr == '\n'
		|| *nptr == '\r' || *nptr == '\t' || *nptr == '\v')
		nptr++;
	is_negative = 0;
	if (*nptr == '-')
		is_negative = 1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	a = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		a = a * 10 + (*nptr - '0');
		nptr++;
	}
	if (is_negative == 1)
		return (-a);
	return (a);
}

static int	check_long_overflow(const char *nptr, int is_negative)
{
	int	len;

	while (*nptr == '0')
	{
		nptr++;
		if (*nptr < '0' || *nptr > '9')
			return (1);
	}
	len = 0;
	while (nptr[len] >= '0' && nptr[len] <= '9')
		len++;
	if (len > 10 || len <= 0)
		return (0);
	if (len < 10)
		return (1);
	if (is_negative == 1)
		return (ft_strncmp(nptr, "9223372036854775808", 19) <= 0);
	return (ft_strncmp(nptr, "9223372036854775807", 19) <= 0);
}

int	ft_atol_valid(const char *nptr)
{
	int	is_negative;

	if (nptr == NULL)
		return (0);
	while (*nptr == ' ' || *nptr == '\f' || *nptr == '\n'
		|| *nptr == '\r' || *nptr == '\t' || *nptr == '\v')
		nptr++;
	is_negative = 0;
	if (*nptr == '-')
		is_negative = 1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	return (check_long_overflow(nptr, is_negative));
}

long	ft_atol_safe(const char *nptr, long error_ret_val)
{
	if (ft_atol_valid(nptr) == 0)
		return (error_ret_val);
	return (ft_atol(nptr));
}
