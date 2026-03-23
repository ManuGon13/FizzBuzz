/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:25:40 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/23 19:24:06 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_word(char *input, int *i, t_token **tokens)
{
	int		start;
	int		end;
	char	*word;
	t_token	*new_tok;

	start = *i;
	end = check_quotes(input, i);
	word = ft_substr(input, start, end - start);
	if (!word)
		return (0);
	new_tok = token_new(word, WORD);
	if (!new_tok)
		return (0);
	token_add_back(tokens, new_tok);
	return (1);
}

static int	handle_operator(char *input, int *i, t_token **tokens)
{
	t_token	*new_tok;

	new_tok = NULL;
	if (input[*i] == '|')
		new_tok = token_new(ft_strdup("|"), PIPE);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		new_tok = token_new(ft_strdup("<<"), HEREDOC);
	else if (input[*i] == '<')
		new_tok = token_new(ft_strdup("<"), REDIR_IN);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		new_tok = token_new(ft_strdup(">>"), APPEND);
	else if (input[*i] == '>')
		new_tok = token_new(ft_strdup(">"), REDIR_OUT);
	if (!new_tok)
		return (0);
	token_add_back(tokens, new_tok);
	if (input[*i] == '|')
		(*i)++;
	else if (input[*i + 1] == input[*i])
		*i += 2;
	else
		(*i)++;
	return (1);
}

int	which_token(char *input, t_token **tokens)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (!handle_operator(input, &i, tokens))
				return (free_tokens(*tokens), 1);
		}
		else
		{
			if (!read_word(input, &i, tokens))
				return (free_tokens(*tokens), 1);
		}
	}
	return (0);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;

	if (!input)
		return (NULL);
	if (has_unclosed_quote(input))
	{
		printf("syntax error: unclosed quote\n");
		return (NULL);
	}
	tokens = NULL;
	if (which_token(input, &tokens))
		return (NULL);
	return (tokens);
}
