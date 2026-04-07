/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 18:53:28 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/20 20:06:36 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	struct_exec_init(t_exec *exec, t_env *env, int *fd, int prev_fd)
{
	exec->env = env;
	exec->prev_fd = prev_fd;
	exec->fd = fd;
}
