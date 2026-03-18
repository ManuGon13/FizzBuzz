/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:25:34 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/18 18:56:17 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_array(t_env *env)
{
	char	**array;
	char	*tmp;
	int		i;
	int		count;
	t_env	*current;

	count = env_size(env);
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	process_exec(int *fd, int prev_fd, t_cmd *cmd, t_env *env)
{
	char	**envp;

	envp = env_to_array(env);
	if (prev_fd != STDIN_FILENO)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	prepare_exec(cmd, envp);
	execve(cmd->argv[0], cmd->argv, envp);
	perror("execve");
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

void	execution(t_cmd *cmd, t_env *env)
{
	pid_t	pid1;
	int		fd[2];
	int		prev_fd;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	prev_fd = STDIN_FILENO;
	while (cmd)
	{
		if (cmd->next && pipe(fd) < 0)
			return ;
		pid1 = fork();
		if (pid1 < 0)
			return ;
		if (pid1 == 0)
			process_exec(fd, prev_fd, cmd, env);
		closing(cmd, &prev_fd, fd);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}
