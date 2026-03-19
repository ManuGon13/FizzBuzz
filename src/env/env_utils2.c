/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:37:13 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/19 17:57:03 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_partial_array(char **array, int filled)
{
	while (filled > 0)
	{
		filled--;
		free(array[filled]);
	}
	free(array);
}

char	**env_to_array(t_env *env)
{
	char	**array;
	char	*tmp;
	int		i;
	t_env	*current;

	array = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
			return (free_partial_array(array, i), NULL);
		array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!array[i])
			return (free_partial_array(array, i), NULL);
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
