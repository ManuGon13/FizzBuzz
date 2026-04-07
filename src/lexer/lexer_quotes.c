/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 19:18:18 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/23 19:28:18 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *input, int *i)
{
	char	quote;

	quote = 0;
	while (input[*i])
	{
		if ((input[*i] == '\'' || input[*i] == '"') && quote == 0)
			quote = input[*i];
		else if (input[*i] == quote)
			quote = 0;
		else if (quote == 0 && (input[*i] == ' ' || input[*i] == '|'
				|| input[*i] == '<' || input[*i] == '>'))
			break ;
		(*i)++;
	}
	return (*i);
}

int	has_unclosed_quote(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"') && quote == 0)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		i++;
	}
	return (quote != 0);
}
