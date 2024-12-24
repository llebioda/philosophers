/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:35:11 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/17 11:29:25 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long	ft_atol(const char *nptr)
{
	long	a;
	int		is_neg;

	while (*nptr == ' ' || *nptr == '\f' || *nptr == '\n'
		|| *nptr == '\r' || *nptr == '\t' || *nptr == '\v')
		nptr++;
	is_neg = 0;
	if (*nptr == '-')
		is_neg = 1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	a = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		a = a * 10 + (*nptr - '0');
		nptr++;
	}
	if (is_neg == 1)
		return (-a);
	return (a);
}
