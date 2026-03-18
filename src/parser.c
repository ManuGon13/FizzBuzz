/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:54:02 by egonin            #+#    #+#             */
/*   Updated: 2026/03/18 17:21:49 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_argv(char **argv)
{
	int	len;

	len = 0;
	if (argv)
	{
		while (argv[len])
			len++;
	}
	return (len);
}

void	add_arg(t_cmd *cmd, char *arg)
{
	char	**new_argv;
	int		len;
	int		i;

	if (!cmd || !arg)
		return ;
	len = count_argv(cmd->argv);
	new_argv = malloc((len + 2) * sizeof(char *));
	if (!new_argv)
		return ;
	i = 0;
	while (i < len)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	if (!new_argv[i])
		return (free(new_argv), (void)0);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

static void	handle_redir(t_cmd *cmd, t_token **tokens)
{
	if ((*tokens)->type == REDIR_IN)
	{
		if (!(*tokens)->next || (*tokens)->next->type != WORD)
			return ;
		free(cmd->infile);
		cmd->infile = ft_strdup((*tokens)->next->value);
		*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
	{
		if (!(*tokens)->next || (*tokens)->next->type != WORD)
			return ;
		free(cmd->outfile);
		cmd->outfile = ft_strdup((*tokens)->next->value);
		cmd->append = ((*tokens)->type == APPEND);
		*tokens = (*tokens)->next;
	}
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*tmp;

	tmp = new_cmd();
	cmd = tmp;
	while (tokens)
	{
		if (tokens->type == WORD)
			add_arg(cmd, tokens->value);
		else if (tokens->type == PIPE)
		{
			cmd->next = new_cmd();
			cmd = cmd->next;
		}
		else
			handle_redir(cmd, &tokens);
		tokens = tokens->next;
	}
	return (tmp);
}
