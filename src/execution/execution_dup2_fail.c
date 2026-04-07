/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_dup2_fail.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:36:00 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/24 18:44:34 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup2_or_fail(int oldfd, int newfd, char **envp, int *fds_to_close)
{
	if (dup2(oldfd, newfd) < 0)
	{
		if (fds_to_close && fds_to_close[0] >= 0)
			close(fds_to_close[0]);
		if (fds_to_close && fds_to_close[1] >= 0)
			close(fds_to_close[1]);
		perror("dup2");
		free_split(envp);
		exit(1);
	}
}
