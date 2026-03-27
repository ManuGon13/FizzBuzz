/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:08:08 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/27 17:03:24 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_outfile(t_exec *exec, t_cmd *cmd, char **envp)
{
	int	outfile;

	if (cmd->append)
		outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
	{
		outfile_fail(cmd, exec->fd, exec->prev_fd, envp);
		exit(1);
	}
	dup2_or_fail(outfile, STDOUT_FILENO, envp, (int [2]){outfile, -1});
	close(outfile);
	if (cmd->next)
	{
		close(exec->fd[1]);
		close(exec->fd[0]);
	}
}

void	process_outfile_next(t_exec *exec, t_cmd *cmd, char **envp)
{
	if (cmd->outfile)
		process_outfile(exec, cmd, envp);
	else if (cmd->next)
	{
		dup2_or_fail(exec->fd[1], STDOUT_FILENO, envp,
			(int [2]){exec->fd[1], exec->fd[0]});
		close(exec->fd[1]);
		close(exec->fd[0]);
	}
	finish_execution(exec, cmd, envp);
}

static void	process_stdin(t_exec *exec, t_cmd *cmd, char **envp)
{
	int	infile;

	if (cmd->heredoc_fd >= 0)
	{
		dup2_or_fail(cmd->heredoc_fd, STDIN_FILENO, envp,
			(int [2]){cmd->heredoc_fd, -1});
		close_heredoc_fd(&cmd->heredoc_fd);
	}
	else if (cmd->infile)
	{
		infile = open(cmd->infile, O_RDONLY);
		if (infile == -1)
		{
			infile_fail(cmd, exec->fd, exec->prev_fd, envp);
			exit(1);
		}
		dup2_or_fail(infile, STDIN_FILENO, envp, (int [2]){infile, -1});
		close(infile);
	}
	else if (exec->prev_fd != STDIN_FILENO)
	{
		dup2_or_fail(exec->prev_fd, STDIN_FILENO, envp,
			(int [2]){exec->prev_fd, -1});
		close(exec->prev_fd);
	}
}

void	process_exec(t_exec *exec, t_cmd *cmd)
{
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	envp = env_to_array(exec->env);
	if (!envp)
	{
		perror("malloc");
		exit(1);
	}
	process_stdin(exec, cmd, envp);
	process_outfile_next(exec, cmd, envp);
}
