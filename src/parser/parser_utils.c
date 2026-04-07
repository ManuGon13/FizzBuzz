/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 18:24:23 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/27 16:51:25 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*res;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	res = malloc((ft_strlen(str) + 1) * (sizeof(char)));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
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
	cmd->heredoc_delim = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->out_append = NULL;
	cmd->infile_count = 0;
	cmd->outfile_count = 0;
	return (cmd);
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

int	count_redirs(t_token *tokens, t_token_type type1, t_token_type type2)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == type1 || tokens->type == type2)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	init_redir_arrays(t_cmd *cmd, t_token *tokens)
{
	int	in_count;
	int	out_count;

	in_count = count_redirs(tokens, REDIR_IN, HEREDOC);
	out_count = count_redirs(tokens, REDIR_OUT, APPEND);
	cmd->infiles = ft_calloc(in_count + 1, sizeof(char *));
	cmd->outfiles = ft_calloc(out_count + 1, sizeof(char *));
	cmd->out_append = ft_calloc(out_count + 1, sizeof(int));
	if (!cmd->infiles || !cmd->outfiles || !cmd->out_append)
	{
		free(cmd->infiles);
		free(cmd->outfiles);
		free(cmd->out_append);
		cmd->infiles = NULL;
		cmd->outfiles = NULL;
		cmd->out_append = NULL;
		return (0);
	}
	cmd->infile_count = 0;
	cmd->outfile_count = 0;
	return (1);
}
