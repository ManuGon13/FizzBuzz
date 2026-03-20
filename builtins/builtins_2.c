/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 17:34:37 by egonin            #+#    #+#             */
/*   Updated: 2026/03/20 19:09:29 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_cmd *cmd, t_env **env)

int	is_valid_identifier(char *str)
{
	int	i;
	
	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if ((str[0] >= '0' && str[0] <= '9') || str[0] == '=' || str[0] == '_') 
		return (0);
	i = 1;
	while (str[i])
	{
		if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_'))
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

void	print_export(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			ft_printf("declare -x %s\n", env->key);
		env = env->next;
	}
}

char	*extract_key(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, "=");
	if (equal)
		return(ft_substr(arg, 0, equal - arg));
	else
		return(ft_strdup(arg));
}

char	*extract_value(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, "=");
	if (equal)
		return(ft_strdup(equal + 1));
	else
		return(NULL);
}