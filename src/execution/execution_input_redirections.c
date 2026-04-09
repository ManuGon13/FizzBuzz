/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_input_redirections.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:18:12 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/09 17:04:31 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	last_input_heredoc(t_token *tokens)
{
	int	is_heredoc;

	is_heredoc = 0;
	while (tokens && tokens->type != PIPE)
	{
		if ((tokens->type == REDIR_IN || tokens->type == HEREDOC)
			&& tokens->next && tokens->next->type == WORD)
		{
			is_heredoc = (tokens->type == HEREDOC);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (is_heredoc);
}

static int	validate_input_redirections(t_token *tokens)
{
	char	*path;
	int		infile;

	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == REDIR_IN && tokens->next
			&& tokens->next->type == WORD)
		{
			path = remove_quotes(tokens->next->value);
			if (!path)
				return (perror("malloc"), 0);
			infile = open(path, O_RDONLY);
			if (infile < 0)
				return (perror(path), free(path), 0);
			close(infile);
			free(path);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (1);
}

int	apply_stdin_redirection(t_cmd *cmd)
{
	int	infile;

	if (!validate_input_redirections(cmd->tokens))
		return (0);
	if (cmd->heredoc_fd >= 0 && last_input_heredoc(cmd->tokens))
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
			return (perror("dup2"), 0);
		close_heredoc_fd(&cmd->heredoc_fd);
	}
	else
	{
		if (cmd->heredoc_fd >= 0)
			close_heredoc_fd(&cmd->heredoc_fd);
		if (cmd->infile)
		{
			infile = open(cmd->infile, O_RDONLY);
			if (infile < 0)
				return (perror(cmd->infile), 0);
			if (dup2(infile, STDIN_FILENO) < 0)
				return (close(infile), perror("dup2"), 0);
			close(infile);
		}
	}
	return (1);
}
