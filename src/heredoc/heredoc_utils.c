/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 17:33:44 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/14 18:51:04 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

static int	prepare_one_heredoc(t_cmd *cmd, t_token *tmp, t_shell *shell)
{
	int		fd;
	char	*new_delim;

	if (!tmp->next || tmp->next->type != WORD)
		return (1);
	cmd->heredoc_expand = !has_quotes(tmp->next->value);
	new_delim = remove_quotes(tmp->next->value);
	if (!new_delim)
		return (perror("malloc"), 1);
	free(cmd->heredoc_delim);
	cmd->heredoc_delim = new_delim;
	fd = handle_heredoc(cmd, shell);
	if (fd < 0)
	{
		shell->exit_status = 130;
		return (1);
	}
	close_heredoc_fd(&cmd->heredoc_fd);
	cmd->heredoc_fd = fd;
	return (0);
}

int	prepare_cmd_heredocs(t_cmd *cmd, t_shell *shell)
{
	t_token	*tmp;

	tmp = cmd->tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == HEREDOC)
		{
			if (prepare_one_heredoc(cmd, tmp, shell))
				return (1);
		}
		tmp = tmp->next;
	}
	if (cmd->heredoc_fd >= 0 && !last_input_is_heredoc(cmd->tokens))
		close_heredoc_fd(&cmd->heredoc_fd);
	return (0);
}
