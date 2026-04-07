/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:54:02 by egonin            #+#    #+#             */
/*   Updated: 2026/04/03 15:52:04 by ltourbe          ###   ########.fr       */
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

static void	handle_redir(t_cmd *cmd, t_token **tokens)
{
	char	*target;

	if (!*tokens || !(*tokens)->next || (*tokens)->next->type != WORD)
		return ;
	target = remove_quotes((*tokens)->next->value);
	if (!target)
		return ;
	if ((*tokens)->type == REDIR_IN)
	{
		cmd->infiles[cmd->infile_count++] = target;
		cmd->infile = (free(cmd->infile), ft_strdup(target));
	}
	else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
	{
		cmd->outfiles[cmd->outfile_count] = target;
		cmd->out_append[cmd->outfile_count++] = ((*tokens)->type == APPEND);
		cmd->outfile = (free(cmd->outfile), ft_strdup(target));
		cmd->append = ((*tokens)->type == APPEND);
	}
	else if ((*tokens)->type == HEREDOC)
		return (free(target), if_heredoc(tokens, cmd));
	*tokens = (*tokens)->next;
}

static int	parse_loop(t_token *tokens, t_cmd **cmd, t_cmd *tmp, t_shell *shell)
{
	t_token	*start;

	start = tokens;
	while (tokens)
	{
		if (tokens->type == WORD)
		{
			if (token_word(tokens->value, *cmd, tmp, shell))
				return (1);
		}
		else if (tokens->type == PIPE)
		{
			(*cmd)->tokens = start;
			if (token_pipe(cmd, tmp))
				return (1);
			start = tokens->next;
			if (!init_redir_arrays(*cmd, start))
				return (free_cmds(tmp), 1);
		}
		else
			handle_redir(*cmd, &tokens);
		tokens = tokens->next;
	}
	(*cmd)->tokens = start;
	return (0);
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmd;
	t_cmd	*tmp;

	tmp = new_cmd();
	if (!tmp)
		return (NULL);
	if (!init_redir_arrays(tmp, tokens))
		return (free_cmds(tmp), NULL);
	cmd = tmp;
	if (parse_loop(tokens, &cmd, tmp, shell))
		return (NULL);
	return (tmp);
}
