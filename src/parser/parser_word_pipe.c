/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 19:26:24 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/09 15:48:17 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_unquoted_word(char *value, char *clean)
{
	if (!clean || clean[0] != '\0')
		return (0);
	if (ft_strchr(value, '\'') || ft_strchr(value, '"'))
		return (0);
	return (1);
}

void	add_arg_fail(char *clean)
{
	perror("malloc");
	free(clean);
}

int	token_word(char *value, t_cmd *cmd, t_shell *shell)
{
	char	*clean;
	char	*expanded;

	expanded = expand_variables(value, shell->env, shell->exit_status);
	if (!expanded)
	{
		perror("malloc");
		return (1);
	}
	clean = remove_quotes(expanded);
	free(expanded);
	if (!clean)
	{
		perror("malloc");
		return (1);
	}
	if (is_empty_unquoted_word(value, clean))
		return (free(clean), 0);
	if (!add_arg(cmd, clean))
		return (add_arg_fail(clean), 1);
	free(clean);
	return (0);
}

int	token_pipe(t_cmd **cmd)
{
	(*cmd)->next = new_cmd();
	if (!(*cmd)->next)
		return (1);
	*cmd = (*cmd)->next;
	return (0);
}
