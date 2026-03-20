/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:25:34 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/20 20:13:13 by ltourbe          ###   ########.fr       */
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
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
	}
	else if (cmd->next)
	{
		dup2(exec->fd[1], STDOUT_FILENO);
		close(exec->fd[1]);
		close(exec->fd[0]);
	}
	finish_execution(exec, cmd, envp);
}

void	process_exec(t_exec *exec, t_cmd *cmd)
{
	char	**envp;
	int		infile;

	envp = env_to_array(exec->env);
	if (cmd->infile)
	{
		infile = open(cmd->infile, O_RDONLY);
		if (infile == -1)
		{
			infile_fail(cmd, exec->fd, exec->prev_fd, envp);
			exit(1);
		}
		dup2(infile, STDIN_FILENO);
		close(infile);
	}
	else if (exec->prev_fd != STDIN_FILENO)
	{
		dup2(exec->prev_fd, STDIN_FILENO);
		close(exec->prev_fd);
	}
	process_outfile_next(exec, cmd, envp);
}

void	execution(t_cmd *cmd, t_env *env)
{
	pid_t	pid1;
	int		fd[2];
	t_exec	exec;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	struct_exec_init(&exec, env, fd, STDIN_FILENO);
	while (cmd)
	{
		if (is_builtin(cmd) && !cmd->next)
			return (exec_builtin_parent(&exec, cmd));
		if (cmd->next && pipe(fd) < 0)
			return ;
		pid1 = fork();
		if (pid1 < 0)
			return ;
		if (pid1 == 0)
			process_exec(&exec, cmd);
		closing(cmd, &exec.prev_fd, fd);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}
