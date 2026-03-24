/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:08:08 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/24 19:08:34 by ltourbe          ###   ########.fr       */
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
	if (tmp == NULL)
	{
		print_error(cmd->argv[0], 1);
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
		perror("execve");
		free_split(envp);
		exit(1);
	}
}

void	process_outfile_next(t_exec *exec, t_cmd *cmd, char **envp)
{
	int	outfile;

	if (cmd->outfile)
	{
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
	}
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

	if (cmd->infile)
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
