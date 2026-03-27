/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:14:58 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/27 17:34:23 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
}

void	update_pwd(t_env *env)
{
	char	cwd[1024];
	char	*oldpwd;

	oldpwd = get_env_value(env, "PWD");
	if (getcwd(cwd, sizeof(cwd)))
	{
		if (oldpwd)
			update_env(env, "OLDPWD", oldpwd);
		update_env(env, "PWD", cwd);
	}
}

int	home_not_set(char **path, t_env *env)
{
	*path = get_env_value(env, "HOME");
	if (!*path)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	return (0);
}

int	cd_minus(char **path, t_env *env)
{
	*path = get_env_value(env, "OLDPWD");
	if (!*path)
	{
		printf("cd: OLDPWD not set\n");
		return (1);
	}
	printf("%s\n", *path);
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env *env)
{
	char	*path;
	int		res;

	path = NULL;
	if (cmd->argv[1] && cmd->argv[2])
		return (ft_putendl_fd("cd: too many arguments", 2), 1);
	if (cmd->argv[1] && !ft_strcmp(cmd->argv[1], "-"))
	{
		if (cd_minus(&path, env))
			return (1);
	}
	else if (cmd->argv[1])
		path = cmd->argv[1];
	else if (home_not_set(&path, env))
		return (1);
	res = chdir(path);
	if (res != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd(env);
	return (0);
}
