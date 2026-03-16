/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 19:07:27 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/16 19:29:15 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**find_path(char **envp)
{
	char	**paths;

	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			break ;
		envp++;
	}
	if (*envp == NULL)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	return (paths);
}

char	*good_path(char *command, char **paths)
{
	char	*path;
	char	*temp;

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	if (!paths)
		return (NULL);
	while (*paths)
	{
		temp = ft_strjoin(*paths, "/");
		path = ft_strjoin(temp, command);
		free(temp);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		paths++;
	}
	return (NULL);
}

void	prepare_exec(t_cmd *cmd, char **envp)
{
	char	**path;
	char	*tmp;

	if (!cmd->argv || !cmd->argv[0])
		return ;
	path = find_path(envp);
	tmp = good_path(cmd->argv[0], path);
	if (path != NULL)
		free_split(path);
	if (tmp == NULL)
	{
		print_error(cmd->argv[0], 1);
		exit(127);
	}
	free(cmd->argv[0]);
	cmd->argv[0] = tmp;
}
