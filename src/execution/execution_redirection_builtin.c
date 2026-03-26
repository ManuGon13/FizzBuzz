/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection_builtin.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 19:33:45 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/26 17:54:47 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_stdin_redirection(t_cmd *cmd)
{
	int	infile;

	if (cmd->heredoc_delim)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
			return (perror("dup2"), 0);
		close(cmd->heredoc_fd);
	}
	else if (cmd->infile)
	{
		infile = open(cmd->infile, O_RDONLY);
		if (infile < 0)
			return (perror(cmd->infile), 0);
		if (dup2(infile, STDIN_FILENO) < 0)
			return (close(infile), perror("dup2"), 0);
		close(infile);
	}
	return (1);
}

static int	apply_redirections(t_cmd *cmd)
{
	int	outfile;

	if (!apply_stdin_redirection(cmd))
		return (0);
	if (cmd->outfile)
	{
		if (cmd->append)
			outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile < 0)
			return (perror(cmd->outfile), 0);
		if (dup2(outfile, STDOUT_FILENO) < 0)
			return (close(outfile), perror("dup2"), 0);
		close(outfile);
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
	status = exec_builtin(cmd, exec);
	restore_stdio(stdin_backup, stdout_backup);
	return (status);
}
