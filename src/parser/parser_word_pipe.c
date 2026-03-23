/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 19:26:24 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/23 19:28:27 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_word(char *value, t_cmd *cmd, t_cmd *start)
{
	char	*clean;

	clean = remove_quotes(value);
	if (!clean)
	{
		perror("malloc");
		free_cmds(start);
		return (1);
	}
	add_arg(cmd, clean);
	free(clean);
	return (0);
}

int	token_pipe(t_cmd **cmd, t_cmd *start)
{
	(*cmd)->next = new_cmd();
	if (!(*cmd)->next)
	{
		free_cmds(start);
		return (1);
	}
	*cmd = (*cmd)->next;
	return (0);
}
