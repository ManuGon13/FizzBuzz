/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_process_helpers2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:56:21 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/09 17:06:43 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	open_redir_side_effect(t_exec *exec, t_cmd *cmd,
			char **envp, t_token *token);
int		is_file_redir(t_token *token);

int	last_input_is_heredoc(t_token *tokens)
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

void	apply_redir_side_effects(t_exec *exec, t_cmd *cmd, char **envp)
{
	t_token	*tokens;

	tokens = cmd->tokens;
	while (tokens && tokens->type != PIPE)
	{
		if (is_file_redir(tokens)
			&& tokens->next && tokens->next->type == WORD)
		{
			open_redir_side_effect(exec, cmd, envp, tokens);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
}
