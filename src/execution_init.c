/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:25:34 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/17 16:32:57 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

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

void	first_exec(t_cmd *cmd, t_env *env)
{
	char	**envp;

	envp = env_to_array(env);
	prepare_exec(cmd, envp);
	execve(cmd->argv[0], cmd->argv, envp);
	perror("execve");
	free_split(envp);
	exit(1);
}

void	execution(t_cmd *cmd, t_env *env)
{
	pid_t	pid1;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
		first_exec(cmd, env);
	waitpid(pid1, NULL, 0);
}
