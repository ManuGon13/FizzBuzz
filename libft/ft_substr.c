/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:18:35 by egonin            #+#    #+#             */
/*   Updated: 2025/11/12 16:14:59 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

char	*ft_goodsub(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	total;
	size_t	i;

	total = ft_strlen(s) - start;
	if (total > len)
		total = len;
	sub = malloc((total + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < total)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[total] = '\0';
	return (sub);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*dup;
	unsigned int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		dup = malloc(1);
		if (!dup)
			return (NULL);
		dup[0] = '\0';
		return (dup);
	}
	return (ft_goodsub(s, start, len));
}
