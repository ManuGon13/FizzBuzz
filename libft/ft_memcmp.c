/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 18:39:35 by egonin            #+#    #+#             */
/*   Updated: 2025/11/11 16:00:21 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*st1 = (const unsigned char *)s1;
	const unsigned char	*st2 = (const unsigned char *)s2;
	size_t				i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n)
	{
		if (st1[i] != st2[i])
			return ((int)(st1[i] - st2[i]));
		i++;
	}
	return (0);
}
