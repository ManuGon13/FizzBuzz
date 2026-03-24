/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 10:37:10 by egonin            #+#    #+#             */
/*   Updated: 2026/03/24 18:14:19 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	key_found(const char *value, t_env *tmp)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return ;
	free(tmp->value);
	tmp->value = new_value;
	return ;
}

void	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new;
	char	*new_key;
	char	*new_value;

	if (!env || !key || !value)
		return ;
	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (key_found(value, tmp));
		tmp = tmp->next;
	}
	new_key = ft_strdup(key);
	if (!new_key)
		return ;
	new_value = ft_strdup(value);
	if (!new_value)
		return (free(new_key));
	new = env_new(new_key, new_value);
	if (!new)
		return ;
	env_add_back(env, new);
}

void	env_delone(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	env_unset(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev == NULL)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			env_delone(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
