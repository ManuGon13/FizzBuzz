/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_process_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 12:35:00 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/07 12:35:00 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_outfile(char *path, int append)
{
	if (append)
		return (open(path, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644));
}

int	is_file_redir(t_token *token)
{
	if (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == APPEND)
		return (1);
	return (0);
}

void	open_redir_side_effect(t_exec *exec, t_cmd *cmd,
	char **envp, t_token *token)
{
	char	*path;
	int		fd;

	path = remove_quotes(token->next->value);
	if (!path)
	{
		perror("malloc");
		free_split(envp);
		exit(1);
	}
	if (token->type == REDIR_IN)
		fd = open(path, O_RDONLY);
	else
		fd = open_outfile(path, token->type == APPEND);
	if (fd < 0)
	{
		free(path);
		if (token->type == REDIR_IN)
			infile_fail(cmd, exec->fd, exec->prev_fd, envp);
		else
			outfile_fail(cmd, exec->fd, exec->prev_fd, envp);
		return ;
	}
	close(fd);
	free(path);
}

void	redirect_outfile_or_fail(t_exec *exec, t_cmd *cmd,
	char **envp, int outfile)
{
	if (outfile == -1)
	{
		outfile_fail(cmd, exec->fd, exec->prev_fd, envp);
		exit(1);
	}
	dup2_or_fail(outfile, STDOUT_FILENO, envp, (int [2]){outfile, -1});
	close(outfile);
}

void	dup_stdin_or_fail(int fd, char **envp)
{
	int	fds_to_close[2];

	fds_to_close[0] = fd;
	fds_to_close[1] = -1;
	dup2_or_fail(fd, STDIN_FILENO, envp, fds_to_close);
}
