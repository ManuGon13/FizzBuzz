/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:25:40 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/16 16:56:43 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_add_back(t_token **list, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_token	*token_new(char *value, t_token_type type)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		return (NULL);
	}
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

static int	read_word(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*word;
	t_token	*new_tok;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
		(*i)++;
	word = ft_substr(input, (unsigned int)start, (size_t)(*i - start));
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

t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;

	if (!input)
		return (NULL);
	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (!handle_operator(input, &i, &tokens))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (!read_word(input, &i, &tokens))
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}
