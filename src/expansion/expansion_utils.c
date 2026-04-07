/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 17:51:24 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/25 18:16:46 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_char(char c)
{
	if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		|| ((c >= '0' && c <= '9') || c == '_'))
		return (1);
	return (0);
}

void	update_quote(char c, char *quote)
{
	if ((c == '\'' || c == '"') && *quote == 0)
		*quote = c;
	else if (c == *quote)
		*quote = 0;
}

int	get_var_len(char *input, int i)
{
	int	var_len;

	var_len = 0;
	while (input[i + 1 + var_len] && is_var_char(input[i + 1 + var_len]))
		var_len++;
	return (var_len);
}
