/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 17:33:44 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/26 18:43:59 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

int	prepare_cmd_heredocs(t_cmd *cmd, t_shell *shell)
{
	t_token	*tmp;
	int		fd;

	tmp = cmd->tokens;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (!tmp->next || tmp->next->type != WORD)
				return (1);
			cmd->heredoc_expand = !has_quotes(tmp->next->value);
			free(cmd->heredoc_delim);
			cmd->heredoc_delim = remove_quotes(tmp->next->value);
			fd = handle_heredoc(cmd, shell);
			if (fd < 0)
				return (shell->exit_status = 130, 1);
			if (cmd->heredoc_fd != -1)
				close(cmd->heredoc_fd);
			cmd->heredoc_fd = fd;
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (0);
}
