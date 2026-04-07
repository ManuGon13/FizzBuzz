/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:37:58 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/03 14:56:24 by ltourbe          ###   ########.fr       */
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

static int	handle_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		status;

	status = 0;
	key = extract_key(arg);
	value = extract_value(arg);
	if (is_valid_identifier(key))
		env_set(env, key, value);
	else
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("`: not a valid identifier", 2);
		status = 1;
	}
	free(key);
	free(value);
	return (status);
}

int	ft_export(t_cmd *cmd, t_env **env)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	if (!cmd->argv[1])
	{
		print_export(*env);
		return (0);
	}
	while (cmd->argv[i])
	{
		if (handle_export_arg(cmd->argv[i], env))
			status = 1;
		i++;
	}
	return (status);
}
