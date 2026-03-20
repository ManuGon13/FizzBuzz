/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_free_errors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 19:09:42 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/20 18:39:55 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	print_error(char *command, int i)
{
	if (i == 3)
		write (2, "Permission denied", 17);
	if (i == 1 || i == 2)
	{
		if (i == 1)
			write(2, "command not found: ", 19);
		if (i == 2)
			write(2, "no such file or directory: ", 27);
		write(2, command, ft_strlen(command));
	}
	write(2, "\n", 1);
}

void	infile_fail(t_cmd *cmd, int *fd, int prev_fd, char **envp)
{
	print_error(cmd->infile, 2);
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	close(fd[0]);
	close(fd[1]);
	free_split(envp);
	exit(1);
}

void	outfile_fail(t_cmd *cmd, int *fd, int prev_fd, char **envp)
{
	print_error(cmd->outfile, 3);
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	close(fd[0]);
	close(fd[1]);
	free_split(envp);
	exit(1);
}

void	closing(t_cmd *cmd, int *prev_fd, int *fd)
{
	if (*prev_fd != STDIN_FILENO)
		close(*prev_fd);
	if (cmd->next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	else
		*prev_fd = STDIN_FILENO;
}
