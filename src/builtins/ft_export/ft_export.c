/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:37:58 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/24 17:58:01 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (!((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| str[i] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!((str[i] >= '0' && str[i] <= '9')
				|| (str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')
				|| str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

t_env	*find_env_node(t_env *env, char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*extract_key(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
		return (ft_substr(arg, 0, equal - arg));
	else
		return (ft_strdup(arg));
}

char	*extract_value(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
		return (ft_strdup(equal + 1));
	else
		return (NULL);
}

int	ft_export(t_cmd *cmd, t_env **env)
{
	char	*arg;
	char	*key;
	char	*value;
	int		i;

	i = 1;
	if (!cmd->argv[1])
	{
		print_export(*env);
		return (0);
	}
	while (cmd->argv[i])
	{
		arg = cmd->argv[i];
		key = extract_key(arg);
		value = extract_value(arg);
		if (is_valid_identifier(key))
			env_set(env, key, value);
		else
			printf("export: `%s`: not a valid identifier\n", arg);
		free(key);
		free(value);
		i++;
	}
	return (0);
}
