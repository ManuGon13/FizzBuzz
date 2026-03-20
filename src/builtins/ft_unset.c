/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 16:37:33 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/20 19:26:58 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*free_node(t_env *current, t_env **env, t_env *prev)
{
	t_env	*next;

	next = current->next;
	free(current->key);
	free(current->value);
	free(current);
	if (prev == NULL)
		*env = next;
	else
		prev->next = next;
	return (next);
}

int	ft_unset(t_env *env, char **argv)
{
	int		i;
	t_env	*prev;
	t_env	*current;

	i = 1;
	while (argv[i])
	{
		prev = NULL;
		current = env;
		while (current)
		{
			if (!ft_strcmp(argv[i], current->key))
				current = free_node(current, &env, prev);
			else
			{
				prev = current;
				current = current->next;
			}
		}
		i++;
	}
	return (0);
}
