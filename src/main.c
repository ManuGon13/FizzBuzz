/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:22:07 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/23 17:48:15 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(char *line, t_token *tokens, t_cmd *cmds, t_shell *shell)
{
	tokens = lexer(line);
	free(line);
	cmds = parser(tokens, shell);
	if (!cmds)
	{
		free_tokens(tokens);
		return ;
	}
	if (prepare_heredocs(cmds, shell))
	{
		free_tokens(tokens);
		free_cmds(cmds);
		return ;
	}
	execution(cmds, shell);
	free_cmds(cmds);
	free_tokens(tokens);
}

void	handle_signal(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	shell_loop(t_shell *shell)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = NULL;
	cmds = NULL;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		minishell(line, tokens, cmds, shell);
		if (shell->should_exit)
			break ;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.env = env_init(envp);
	shell.exit_status = 0;
	shell.should_exit = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(&shell);
	clear_history();
	free_env(shell.env);
	return (shell.exit_status);
}
