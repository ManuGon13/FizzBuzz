/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection_builtin.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 19:33:45 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/03 16:17:33 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_stdout(char *path, int append)
{
	int	fd;

	if (append)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(path), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (close(fd), perror("dup2"), 0);
	close(fd);
	return (1);
}

static int	apply_redirections(t_cmd *cmd)
{
	int	i;

	if (!apply_stdin_redirection(cmd))
		return (0);
	if (cmd->outfile_count <= 0 || !cmd->outfiles || !cmd->out_append)
		return ((cmd->outfile == NULL)
			|| redirect_stdout(cmd->outfile, cmd->append));
	i = 0;
	while (i < cmd->outfile_count)
	{
		if (!redirect_stdout(cmd->outfiles[i], cmd->out_append[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	backup_stdio(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	if (*stdin_backup < 0)
		return (perror("dup"), 0);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdout_backup < 0)
	{
		close(*stdin_backup);
		return (perror("dup"), 0);
	}
	return (1);
}

static void	restore_stdio(int stdin_backup, int stdout_backup)
{
	if (dup2(stdin_backup, STDIN_FILENO) < 0)
		perror("dup2");
	if (dup2(stdout_backup, STDOUT_FILENO) < 0)
		perror("dup2");
	close(stdin_backup);
	close(stdout_backup);
}

int	exec_builtin_parent(t_exec *exec, t_cmd *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	if (!backup_stdio(&stdin_backup, &stdout_backup))
		return (1);
	if (!apply_redirections(cmd))
		return (restore_stdio(stdin_backup, stdout_backup), 1);
	if (!ft_strcmp(cmd->argv[0], "exit") && (!cmd->argv[1]
			|| cmd->argv[2] == NULL))
	{
		restore_stdio(stdin_backup, stdout_backup);
		exec_builtin(cmd, exec);
		return (1);
	}
	status = exec_builtin(cmd, exec);
	restore_stdio(stdin_backup, stdout_backup);
	return (status);
}
