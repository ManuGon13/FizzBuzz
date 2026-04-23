/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_has_redirection.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:51:33 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/23 17:20:48 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_redirections(t_cmd *cmd)
{
	t_token	*tok;

	if (!cmd)
		return (0);
	if (cmd->heredoc_fd >= 0)
		return (1);
	tok = cmd->tokens;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == REDIR_IN || tok->type == REDIR_OUT
			|| tok->type == APPEND || tok->type == HEREDOC)
			return (1);
		tok = tok->next;
	}
	return (0);
}
