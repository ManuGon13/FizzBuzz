/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:22:07 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/12 12:09:15 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void lexer(char *input)
{
    (void)input;
}

void parser(void)
{
}

void execute(void)
{
}

void    print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("TOKEN: %s\n", tokens->value);
        tokens = tokens->next;
    }
}

void    free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

int main(void)
{
    char    *line;
    t_token *tokens;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            printf("exit\n");
            break ;
        }
        if (line && *line)
            add_history(line);
        tokens = lexer(line);
        print_tokens(tokens);
        free_tokens(tokens);
        parser();
        execute();
        free(line);
    }
    return (0);
}
