/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:15:53 by egonin            #+#    #+#             */
/*   Updated: 2026/03/18 18:52:31 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_env **env)
{
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (ft_cd(cmd, env));
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (ft_env(*env));
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env **env);

void	ft_pwd(void)
{
	char	buf[1024];
	size_t	size;
	char	*pwd;

	size = sizeof(buf);
	pwd = getcwd(buf, size);
	if (pwd)
		ft_printf("%s\n", pwd);
	else
		perror("pwd");
}

int	ft_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}