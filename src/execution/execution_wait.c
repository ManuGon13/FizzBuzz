/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_wait.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:46:18 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/09 17:31:55 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	ft_wait(pid_t last_pid)
{
	int		status;
	int		exit_code;
	pid_t	pid;

	pid = wait(&status);
	exit_code = 0;
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			{
				if (WCOREDUMP(status))
					write(2, "Quit (core dumped)\n", 19);
				else
					write(2, "Quit\n", 5);
			}
			exit_code = get_exit_status(status);
		}
		pid = wait(&status);
	}
	return (exit_code);
}
