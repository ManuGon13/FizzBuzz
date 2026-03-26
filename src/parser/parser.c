/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:54:02 by egonin            #+#    #+#             */
/*   Updated: 2026/03/26 18:46:04 by ltourbe          ###   ########.fr       */
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

int	add_arg(t_cmd *cmd, char *arg)
{
	char	**new_argv;
	int		len;
	int		i;

	if (!cmd || !arg)
		return (0);
	len = count_argv(cmd->argv);
	new_argv = malloc((len + 2) * sizeof(char *));
	if (!new_argv)
		return (0);
	i = 0;
	while (i < len)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	if (!new_argv[i])
		return (free(new_argv), 0);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	return (1);
}

void	if_heredoc(t_token **tokens, t_cmd *cmd)
{
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
		return ;
	free(cmd->heredoc_delim);
	cmd->heredoc_expand = !has_quotes((*tokens)->next->value);
	cmd->heredoc_delim = remove_quotes((*tokens)->next->value);
	*tokens = (*tokens)->next;
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
	else if ((*tokens)->type == HEREDOC)
		if_heredoc(tokens, cmd);
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	t_token	*start;

	tmp = new_cmd();
	if (!tmp)
		return (NULL);
	cmd = tmp;
	start = tokens;
	while (tokens)
	{
		if (tokens->type == WORD)
		{
			if (token_word(tokens->value, cmd, tmp, shell))
				return (NULL);
		}
		else if (tokens->type == PIPE)
		{
			cmd->tokens = start;
			if (token_pipe(&cmd, tmp))
				return (NULL);
			start = tokens->next;
		}
		else
			handle_redir(cmd, &tokens);
		tokens = tokens->next;
	}
	cmd->tokens = start;
	return (tmp);
}
