/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_finish.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:55:31 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/23 16:52:46 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <sys/stat.h>

static char	**build_sh_argv(char **argv)
{
	char	**sh_argv;
	int		len;
	int		i;

	len = 0;
	while (argv[len])
		len++;
	sh_argv = malloc(sizeof(char *) * (len + 2));
	if (!sh_argv)
		return (NULL);
	sh_argv[0] = "/bin/sh";
	i = 0;
	while (i < len)
	{
		sh_argv[i + 1] = argv[i];
		i++;
	}
	sh_argv[i + 1] = NULL;
	return (sh_argv);
}

static void	prepare_exec_fail(char *command, char **envp, int not_found)
{
	if (not_found)
		print_error(command, 1);
	else
		perror(command);
	free_split(envp);
	close_child_stdio();
	if (not_found)
		exit(127);
	exit(126);
}

void	prepare_exec(t_cmd *cmd, char **envp)
{
	char		**path;
	char		*tmp;
	struct stat	st;

	if (!cmd->argv || !cmd->argv[0])
		return ;
	if (ft_strchr(cmd->argv[0], '/')
		&& stat(cmd->argv[0], &st) == 0 && S_ISDIR(st.st_mode))
		return (errno = EISDIR, prepare_exec_fail(cmd->argv[0], envp, 0));
	path = find_path(envp);
	tmp = good_path(cmd->argv[0], path);
	if (path != NULL)
		free_split(path);
	if (tmp == (char *)-1)
		return (prepare_exec_fail(cmd->argv[0], envp, 0));
	if (tmp == NULL)
		return (prepare_exec_fail(cmd->argv[0], envp, 1));
	free(cmd->argv[0]);
	cmd->argv[0] = tmp;
}

static void	exec_external(t_cmd *cmd, char **envp)
{
	char	**sh_argv;

	prepare_exec(cmd, envp);
	execve(cmd->argv[0], cmd->argv, envp);
	if (errno != ENOEXEC)
		return ;
	sh_argv = build_sh_argv(cmd->argv);
	if (!sh_argv)
	{
		perror("malloc");
		free_split(envp);
		close_child_stdio();
		exit(1);
	}
	execve("/bin/sh", sh_argv, envp);
	free(sh_argv);
}

void	finish_execution(t_exec *exec, t_cmd *cmd, char **envp)
{
	int		exit_code;

	if (is_builtin(cmd))
	{
		exit_code = exec_builtin(cmd, exec);
		free_split(envp);
		close_child_stdio();
		exit(exit_code);
	}
	exec_external(cmd, envp);
	perror(cmd->argv[0]);
	free_split(envp);
	close_child_stdio();
	exit(1);
}
