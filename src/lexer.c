/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:22:41 by egonin            #+#    #+#             */
/*   Updated: 2026/03/16 15:23:39 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_add_back(t_token **list, t_token *new)
{
	t_token	*tmp;

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

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	read_word(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*word;

	start = *i;
	while ((input[*i] && input[*i] != ' ')
		&& (input[*i] != '|' && input[*i] != '<' && input[*i] != '>'))
		(*i)++;
	word = ft_substr(input, start, *i - start);
	token_add_back(tokens, token_new(word, WORD));
}

void	handle_operator(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
		token_add_back(tokens, token_new(ft_strdup("|"), PIPE));
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
			token_add_back(tokens, token_new(ft_strdup("<<"), HEREDOC));
		else
			token_add_back(tokens, token_new(ft_strdup("<"), REDIR_IN));
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
			token_add_back(tokens, token_new(ft_strdup(">>"), APPEND));
		else
			token_add_back(tokens, token_new(ft_strdup(">"), REDIR_OUT));
	}
	if (input[*i] == '|')
		(*i)++;
	else if (input[*i + 1] == input[*i])
		*i += 2;
	else
		(*i)++;
}

t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			handle_operator(input, &i, &tokens);
		else
			read_word(input, &i, &tokens);
	}
	return (tokens);
}
