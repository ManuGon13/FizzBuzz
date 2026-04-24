/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 00:00:00 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/24 17:33:19 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	syntax_error_token(t_shell *shell, char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	shell->exit_status = 2;
	return (1);
}

static int	is_redir_token(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == APPEND || type == HEREDOC);
}

int	validate_pipe_syntax(t_token *tokens, t_shell *shell)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!prev || (tokens->next && tokens->next->type == PIPE))
				return (syntax_error_token(shell, "|"));
			if (!tokens->next)
				return (syntax_error_token(shell, NULL));
		}
		else if (is_redir_token(tokens->type))
		{
			if (!tokens->next)
				return (syntax_error_token(shell, NULL));
			if (tokens->next->type != WORD)
				return (syntax_error_token(shell, tokens->next->value));
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}
