/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:22:07 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/16 19:27:16 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parser(void)
{
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TOKEN: %s\n", tokens->value);
		tokens = tokens->next;
	}
}

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	t_env	env;

	env = init_env(envp);
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
		tokens = lexer(line);
		free(line);
		print_tokens(tokens);
		free_tokens(tokens);
		parser();
		execution(cmds, env);
	}
	clear_history();
	return (0);
}
