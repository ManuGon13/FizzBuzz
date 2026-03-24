/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:25:34 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/24 18:27:38 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execution_error_cleanup(t_exec *exec, int *fd, int has_pipe)
{
	if (exec->prev_fd != STDIN_FILENO)
		close(exec->prev_fd);
	if (has_pipe)
	{
		close(fd[0]);
		close(fd[1]);
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
}

static int	launch_command(t_exec *exec, t_cmd *cmd, int *fd)
{
	pid_t	pid1;
	int		has_pipe;

	has_pipe = 0;
	if (cmd->next && pipe(fd) < 0)
		return (execution_error_cleanup(exec, fd, has_pipe), 1);
	if (cmd->next)
		has_pipe = 1;
	pid1 = fork();
	if (pid1 < 0)
		return (execution_error_cleanup(exec, fd, has_pipe), 1);
	if (pid1 == 0)
		process_exec(exec, cmd);
	closing(cmd, &exec->prev_fd, fd);
	return (0);
}

void	execution(t_cmd *cmd, t_env *env)
{
	int		fd[2];
	t_exec	exec;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	struct_exec_init(&exec, env, fd, STDIN_FILENO);
	if (is_builtin(cmd) && !cmd->next)
		return (exec_builtin_parent(&exec, cmd));
	while (cmd)
	{
		if (launch_command(&exec, cmd, fd))
			return ;
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}
