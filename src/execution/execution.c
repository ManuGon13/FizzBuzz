/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:25:34 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/19 16:42:09 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finish_execution(t_cmd *cmd, char **envp)
{
	prepare_exec(cmd, envp);
	execve(cmd->argv[0], cmd->argv, envp);
	perror("execve");
	free_split(envp);
	exit(1);
}

void	process_outfile_next(t_cmd *cmd, int *fd, char **envp, int prev_fd)
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
			outfile_fail(cmd, fd, prev_fd, envp);
			exit(1);
		}
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
	}
	else if (cmd->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	finish_execution(cmd, envp);
}

void	process_exec(int *fd, int prev_fd, t_cmd *cmd, t_env *env)
{
	char	**envp;
	int		infile;

	envp = env_to_array(env);
	if (cmd->infile)
	{
		infile = open(cmd->infile, O_RDONLY);
		if (infile == -1)
		{
			infile_fail(cmd, fd, prev_fd, envp);
			exit(1);
		}
		dup2(infile, STDIN_FILENO);
		close(infile);
	}
	else if (prev_fd != STDIN_FILENO)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	process_outfile_next(cmd, fd, envp, prev_fd);
}

void	closing(t_cmd *cmd, int *prev_fd, int *fd)
{
	if (*prev_fd != STDIN_FILENO)
		close(*prev_fd);
	if (cmd->next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	else
		*prev_fd = STDIN_FILENO;
}

void	execution(t_cmd *cmd, t_env *env)
{
	pid_t	pid1;
	int		fd[2];
	int		prev_fd;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	prev_fd = STDIN_FILENO;
	while (cmd)
	{
		if (cmd->next && pipe(fd) < 0)
			return ;
		pid1 = fork();
		if (pid1 < 0)
			return ;
		if (pid1 == 0)
			process_exec(fd, prev_fd, cmd, env);
		closing(cmd, &prev_fd, fd);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}
