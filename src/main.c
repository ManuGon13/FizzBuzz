/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:22:07 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/17 17:46:11 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(char *line, t_token *tokens, t_cmd *cmds, t_env *env)
{
	tokens = lexer(line);
	free(line);
	// print_tokens(tokens);
	cmds = parser(tokens);
	free_tokens(tokens);
	execution(cmds, env);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	t_env	*env;

	(void)ac;
	(void)av;
	env = env_init(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		minishell(line, tokens, cmds, env);
	}
	clear_history();
	return (0);
}
