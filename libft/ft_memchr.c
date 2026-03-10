/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:21:43 by egonin            #+#    #+#             */
/*   Updated: 2025/11/11 15:59:14 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p = (const unsigned char *)s;
	unsigned char		uc;

	uc = (unsigned char)c;
	while (n > 0)
	{
		if (*p == uc)
			return ((void *)p);
		p++;
		n--;
	}
	return (NULL);
}
