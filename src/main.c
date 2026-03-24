/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:22:07 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/24 17:03:47 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(char *line, t_token *tokens, t_cmd *cmds, t_env *env)
{
	tokens = lexer(line);
	free(line);
	cmds = parser(tokens);
	free_tokens(tokens);
	if (!cmds)
		return ;
	execution(cmds, env);
	free_cmds(cmds);
}

void	handle_signal(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	t_env	*env;

	(void)ac;
	(void)av;
	tokens = NULL;
	cmds = NULL;
	env = env_init(envp);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		minishell(line, tokens, cmds, env);
	}
	clear_history();
	free_env(env);
	return (0);
}
