/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:16:45 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/23 16:24:12 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	buf[1024];
	size_t	size;
	char	*pwd;

	size = sizeof(buf);
	pwd = getcwd(buf, size);
	if (pwd)
		printf("%s\n", pwd);
	else
		perror("pwd");
	return (0);
}
