/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 16:37:14 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/20 18:27:47 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	argv_parsing(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if ((argv[i] < '0' || argv[i] > '9'))
		{
			write(2, "error: wrong argument\n", 22);
			exit(2);
		}
		i++;
	}
}

void	ft_exit(char **argv)
{
	int	i;
	int	av;

	if (!argv[1])
		exit(0);
	i = 1;
	while (argv[i])
	{
		if (i > 1)
		{
			write(2, "error: too many arguments\n", 26);
			return ;
		}
		i++;
	}
	i = 0;
	if ((argv[1][0] >= '0' && argv[1][0] <= '9') || ((argv[1][0] == '-'
	|| argv[1][0] == '+') && (argv[1][1] >= '0' && argv[1][1] <= '9')))
		i++;
	argv_parsing(&argv[1][i]);
	av = ft_atoi(argv[1]);
	exit((av % 256 + 256) % 256);
}
