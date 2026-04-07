/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 16:32:45 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/27 17:03:21 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redir_arrays(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->infiles && i < cmd->infile_count)
	{
		free(cmd->infiles[i]);
		i++;
	}
	free(cmd->infiles);
	i = 0;
	while (cmd->outfiles && i < cmd->outfile_count)
	{
		free(cmd->outfiles[i]);
		i++;
	}
	free(cmd->outfiles);
	free(cmd->out_append);
}

void	close_heredoc_fd(int *fd)
{
	if (!fd || *fd < 0)
		return ;
	close(*fd);
	*fd = -1;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free(cmds->infile);
		free(cmds->outfile);
		close_heredoc_fd(&cmds->heredoc_fd);
		free(cmds->heredoc_delim);
		free_split(cmds->argv);
		free_redir_arrays(cmds);
		free(cmds);
		cmds = tmp;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->value);
		free(env->key);
		free(env);
		env = tmp;
	}
}
