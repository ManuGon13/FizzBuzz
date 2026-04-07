/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:05:33 by egonin            #+#    #+#             */
/*   Updated: 2026/03/19 17:54:17 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

t_env	*env_new(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
	{
		free(key);
		free(value);
		return (NULL);
	}
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

int	env_init_protect(char *value, char *key, t_env **env)
{
	t_env	*new;

	if (!value)
	{
		free(key);
		return (free_env(*env), 0);
	}
	new = env_new(key, value);
	if (!new)
		return (free_env(*env), 0);
	env_add_back(env, new);
	return (1);
}

t_env	*env_init(char **envp)
{
	int		i;
	t_env	*env;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '=') != NULL)
		{
			equal = ft_strchr(envp[i], '=');
			key = ft_substr(envp[i], 0, equal - envp[i]);
			if (!key)
				return (free_env(env), NULL);
			value = ft_strdup(equal + 1);
			if (!env_init_protect(value, key, &env))
				return (NULL);
		}
		i++;
	}
	return (env);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
