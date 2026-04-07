/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:48:41 by egonin            #+#    #+#             */
/*   Updated: 2026/03/25 20:00:37 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

int	is_sep(char c, char sep)
{
	return (c == sep);
}

int	count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && is_sep(str[i], c))
			i++;
		if (str[i])
		{
			count++;
			while (str[i] && !is_sep(str[i], c))
				i++;
		}
	}
	return (count);
}

char	*word_dup(char const *str, char c)
{
	int		i;
	int		len;
	char	*word;

	i = 0;
	len = 0;
	while (str[len] && is_sep(str[len], c) == 0)
		len++;
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**free_split_tab(char **tab, int size)
{
	while (size > 0)
	{
		size--;
		free(tab[size]);
	}
	free(tab);
	return (NULL);
}

char	**ft_split(char const *str, char c)
{
	int		i;
	int		j;
	char	**tab;

	i = 0;
	j = 0;
	tab = malloc((count_words(str, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	while (str[i])
	{
		while (str[i] && is_sep(str[i], c) == 1)
			i++;
		if (str[i])
		{
			tab[j] = word_dup(&str[i], c);
			if (!tab[j])
				return (free_split_tab(tab, j));
			j++;
			while (str[i] && is_sep(str[i], c) == 0)
				i++;
		}
	}
	tab[j] = NULL;
	return (tab);
}
