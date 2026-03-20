/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection_builtin.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 19:33:45 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/20 20:07:13 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_cmd *cmd)
{
	int	infile;
	int	outfile;

	if (cmd->infile)
	{
		infile = open(cmd->infile, O_RDONLY);
		dup2(infile, STDIN_FILENO);
		close(infile);
	}
	if (cmd->outfile)
	{
		if (cmd->append)
			outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
	}
}

void	exec_builtin_parent(t_exec *exec, t_cmd *cmd)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	apply_redirections(cmd);
	exec_builtin(cmd, exec);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}
