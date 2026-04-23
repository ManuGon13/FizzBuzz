/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:28:41 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/23 16:52:54 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_future_heredoc_fds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_fd >= 0)
			close_heredoc_fd(&cmd->heredoc_fd);
		cmd = cmd->next;
	}
}

void	close_next_heredocs(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd->next;
	while (tmp)
	{
		if (tmp->heredoc_fd >= 0)
			close_heredoc_fd(&tmp->heredoc_fd);
		tmp = tmp->next;
	}
}

void	close_child_stdio(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
