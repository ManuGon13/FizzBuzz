/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 18:06:38 by egonin            #+#    #+#             */
/*   Updated: 2026/03/23 20:06:42 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_char(char c)
{
	if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		|| ((c >= '0' && c <= '9') || c == '_'))
		return (1);
	return (0);
}

static int	get_var_len(char *input, int i)
{
	int	var_len;

	var_len = 0;
	while (input[i + 1 + var_len] && is_var_char(input[i + 1 + var_len]))
		var_len++;
	return (var_len);
}

static char	*replace_var(char *input, t_env *env, int i, int var_len)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	char	*result;

	var_name = ft_substr(input, i + 1, var_len);
	value = get_env_value(env, var_name);
	if (!value)
		value = "";
	tmp = ft_substr(input, 0, i);
	result = ft_strjoin(tmp, value);
	free(tmp);
	tmp = result;
	result = ft_strjoin(tmp, input + i + 1 + var_len);
	free(tmp);
	free(var_name);
	return (result);
}

char	*expand_variables(char *input, t_env *env)
{
	int		i;
	int		var_len;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			var_len = get_var_len(input, i);
			if (var_len == 0)
				return (ft_strdup(input));
			return (replace_var(input, env, i, var_len));
		}
		i++;
	}
	return (ft_strdup(input));
}
