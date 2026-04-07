/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:01:53 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/26 18:43:59 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static int	heredoc_parent_wait(pid_t pid, int *fd)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	close(fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_signal);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) != 0))
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}

static int	heredoc_loop(t_cmd *cmd, t_shell *shell, int write_fd)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (0);
		if (!ft_strcmp(line, cmd->heredoc_delim))
			return (free(line), 0);
		if (cmd->heredoc_expand)
		{
			expanded = expand_variables(line, shell->env, shell->exit_status);
			free(line);
			line = expanded;
			if (!line)
				return (1);
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(t_cmd *cmd, t_shell *shell)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (close(fd[0]), close(fd[1]), -1);
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint);
		signal(SIGQUIT, SIG_IGN);
		close(fd[0]);
		exit(heredoc_loop(cmd, shell, fd[1]));
	}
	return (heredoc_parent_wait(pid, fd));
}

int	prepare_heredocs(t_cmd *cmd, t_shell *shell)
{
	while (cmd)
	{
		if (prepare_cmd_heredocs(cmd, shell))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
