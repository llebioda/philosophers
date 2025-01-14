/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:43:48 by llebioda          #+#    #+#             */
/*   Updated: 2025/01/10 12:42:37 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	s;
	char	*c;
	size_t	i;

	if (nmemb > 0 && SIZE_MAX / nmemb < size)
		return (NULL);
	s = nmemb * size;
	ptr = malloc(s);
	if (ptr == NULL)
		return (NULL);
	c = ptr;
	i = 0;
	while (i < s)
		c[i++] = 0;
	return (ptr);
}
