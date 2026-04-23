/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 16:37:14 by ltourbe           #+#    #+#             */
/*   Updated: 2026/04/23 17:44:24 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	argv_parsing(char *argv, char *full_arg)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if ((argv[i] < '0' || argv[i] > '9'))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(full_arg, 2);
			ft_putendl_fd(": numeric argument required", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_exit(char **argv)
{
	int	i;
	int	av;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (i > 1)
		{
			write(2, "error: too many arguments\n", 26);
			return (1);
		}
		i++;
	}
	i = 0;
	if ((argv[1][0] >= '0' && argv[1][0] <= '9') || ((argv[1][0] == '-'
	|| argv[1][0] == '+') && (argv[1][1] >= '0' && argv[1][1] <= '9')))
		i++;
	if (!argv_parsing(&argv[1][i], argv[1]))
		return (2);
	av = ft_atoi(argv[1]);
	return ((av % 256 + 256) % 256);
}
