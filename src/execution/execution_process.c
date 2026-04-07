/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:08:08 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/27 17:03:24 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		open_outfile(char *path, int append);
int		is_file_redir(t_token *token);
void	open_redir_side_effect(t_exec *exec, t_cmd *cmd,
			char **envp, t_token *token);
void	redirect_outfile_or_fail(t_exec *exec, t_cmd *cmd,
			char **envp, int outfile);
void	dup_stdin_or_fail(int fd, char **envp);

static void	apply_redir_side_effects(t_exec *exec, t_cmd *cmd, char **envp)
{
	t_token	*tokens;

	tokens = cmd->tokens;
	while (tokens && tokens->type != PIPE)
	{
		if (is_file_redir(tokens)
			&& tokens->next && tokens->next->type == WORD)
		{
			open_redir_side_effect(exec, cmd, envp, tokens);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
}

static void	process_outfile(t_exec *exec, t_cmd *cmd, char **envp)
{
	int	outfile;
	int	i;

	if (cmd->outfile_count > 0 && cmd->outfiles && cmd->out_append)
	{
		i = 0;
		while (i < cmd->outfile_count)
		{
			outfile = open_outfile(cmd->outfiles[i], cmd->out_append[i]);
			redirect_outfile_or_fail(exec, cmd, envp, outfile);
			i++;
		}
	}
	else
	{
		outfile = open_outfile(cmd->outfile, cmd->append);
		redirect_outfile_or_fail(exec, cmd, envp, outfile);
	}
	if (cmd->next)
	{
		close(exec->fd[1]);
		close(exec->fd[0]);
	}
}

void	process_outfile_next(t_exec *exec, t_cmd *cmd, char **envp)
{
	if (cmd->outfile)
		process_outfile(exec, cmd, envp);
	else if (cmd->next)
	{
		dup2_or_fail(exec->fd[1], STDOUT_FILENO, envp,
			(int [2]){exec->fd[1], exec->fd[0]});
		close(exec->fd[1]);
		close(exec->fd[0]);
	}
	finish_execution(exec, cmd, envp);
}

static void	process_stdin(t_exec *exec, t_cmd *cmd, char **envp)
{
	int	infile;

	if (cmd->heredoc_fd >= 0)
	{
		dup_stdin_or_fail(cmd->heredoc_fd, envp);
		close_heredoc_fd(&cmd->heredoc_fd);
	}
	else if (cmd->infile)
	{
		infile = open(cmd->infile, O_RDONLY);
		if (infile == -1)
		{
			infile_fail(cmd, exec->fd, exec->prev_fd, envp);
			exit(1);
		}
		dup_stdin_or_fail(infile, envp);
		close(infile);
	}
	else if (exec->prev_fd != STDIN_FILENO)
	{
		dup_stdin_or_fail(exec->prev_fd, envp);
		close(exec->prev_fd);
	}
}

void	process_exec(t_exec *exec, t_cmd *cmd)
{
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	envp = env_to_array(exec->env);
	if (!envp)
	{
		perror("malloc");
		exit(1);
	}
	apply_redir_side_effects(exec, cmd, envp);
	process_stdin(exec, cmd, envp);
	process_outfile_next(exec, cmd, envp);
}
