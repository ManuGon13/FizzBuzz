/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:25:34 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/03 15:11:35 by ltourbe          ###   ########.fr       */
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

static pid_t	launch_command(t_exec *exec, t_cmd *cmd, int *fd)
{
	pid_t	pid1;
	int		has_pipe;

	has_pipe = 0;
	fd[0] = -1;
	fd[1] = -1;
	if (cmd->next && pipe(fd) < 0)
		return (execution_error_cleanup(exec, fd, has_pipe), -1);
	if (cmd->next)
		has_pipe = 1;
	pid1 = fork();
	if (pid1 < 0)
		return (execution_error_cleanup(exec, fd, has_pipe), -1);
	if (pid1 == 0)
		process_exec(exec, cmd);
	closing(cmd, &exec->prev_fd, fd);
	return (pid1);
}

static int	exec_single_builtin(t_exec *exec, t_cmd *cmd, t_shell *shell)
{
	shell->exit_status = exec_builtin_parent(exec, cmd);
	shell->env = exec->env;
	return (1);
}

static int	launch_all_commands(t_exec *exec, t_cmd *cmd, int *fd, pid_t *last_pid)
{
	while (cmd)
	{
		*last_pid = launch_command(exec, cmd, fd);
		if (*last_pid < 0)
			return (0);
		cmd = cmd->next;
	}
	return (1);
}

void	execution(t_cmd *cmd, t_shell *shell)
{
	int		fd[2];
	t_exec	exec;
	pid_t	last_pid;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	fd[0] = -1;
	fd[1] = -1;
	struct_exec_init(&exec, shell->env, fd, STDIN_FILENO);
	if (is_builtin(cmd) && !cmd->next && exec_single_builtin(&exec, cmd, shell))
		return ;
	signal(SIGINT, handle_exec_signal);
	signal(SIGQUIT, SIG_IGN);
	if (!launch_all_commands(&exec, cmd, fd, &last_pid))
	{
		shell->exit_status = 1;
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	shell->exit_status = ft_wait(last_pid);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}
