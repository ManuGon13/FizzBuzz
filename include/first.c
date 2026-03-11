/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:22:07 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/11 18:36:08 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main(void)
{
    char    *line;

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
        free(line);
    }
    return (0);
}
