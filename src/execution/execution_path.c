/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 19:07:27 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/25 19:29:46 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**find_path(char **envp)
{
	char	**paths;

	if (!envp)
		return (NULL);
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

static char	*handle_slash(char *command)
{
	if (access(command, F_OK) != 0)
		return (NULL);
	if (access(command, X_OK) != 0)
		return ((char *)-1);
	return (ft_strdup(command));
}

char	*good_path(char *command, char **paths)
{
	char	*path;
	char	*temp;

	if (ft_strchr(command, '/'))
		return (handle_slash(command));
	if (!paths)
		return (NULL);
	while (*paths)
	{
		temp = ft_strjoin(*paths, "/");
		if (!temp)
			return (NULL);
		path = ft_strjoin(temp, command);
		free(temp);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		paths++;
	}
	return (NULL);
}
