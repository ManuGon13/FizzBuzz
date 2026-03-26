/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_finish.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:55:31 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/26 16:09:39 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_exec(t_cmd *cmd, char **envp)
{
	char	**path;
	char	*tmp;

	if (!cmd->argv || !cmd->argv[0])
		return ;
	path = find_path(envp);
	tmp = good_path(cmd->argv[0], path);
	if (path != NULL)
		free_split(path);
	if (tmp == (char *)-1)
	{
		perror(cmd->argv[0]);
		free_split(envp);
		exit(126);
	}
	if (tmp == NULL)
	{
		print_error(cmd->argv[0], 1);
		free_split(envp);
		exit(127);
	}
	free(cmd->argv[0]);
	cmd->argv[0] = tmp;
}

void	finish_execution(t_exec *exec, t_cmd *cmd, char **envp)
{
	int	exit_code;

	if (is_builtin(cmd))
	{
		exit_code = exec_builtin(cmd, exec);
		free_split(envp);
		exit(exit_code);
	}
	else
	{
		prepare_exec(cmd, envp);
		execve(cmd->argv[0], cmd->argv, envp);
		perror(cmd->argv[0]);
		free_split(envp);
		exit(1);
	}
}
