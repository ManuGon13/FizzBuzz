/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:59:42 by ltourbe           #+#    #+#             */
/*   Updated: 2026/03/27 17:31:07 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_var(char *input, t_env *env, int i, int var_len)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	char	*result;

	var_name = ft_substr(input, i + 1, var_len);
	if (!var_name)
		return (free(input), NULL);
	value = get_env_value(env, var_name);
	if (!value)
		value = "";
	tmp = ft_substr(input, 0, i);
	if (!tmp)
		return (free(var_name), free(input), NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	if (!result)
		return (free(var_name), free(input), NULL);
	tmp = result;
	result = ft_strjoin(tmp, input + i + 1 + var_len);
	free(tmp);
	free(var_name);
	free(input);
	return (result);
}

static char	*expand_last_status_var(char *input, int i, int last_status)
{
	char	*value;
	char	*tmp;
	char	*result;

	value = ft_itoa(last_status);
	if (!value)
		return (free(input), NULL);
	tmp = ft_substr(input, 0, i);
	if (!tmp)
		return (free(value), free(input), NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	if (!result)
		return (free(value), free(input), NULL);
	tmp = result;
	result = ft_strjoin(tmp, input + i + 2);
	free(tmp);
	free(value);
	free(input);
	return (result);
}

static char	*handle_dollar(char *tmp, t_env *env, int last_status, int *i)
{
	int		var_len;
	char	*new;

	if (tmp[*i + 1] && tmp[*i + 1] == '?')
		return (expand_last_status_var(tmp, *i, last_status));
	var_len = get_var_len(tmp, *i);
	if (var_len == 0)
	{
		(*i)++;
		return (tmp);
	}
	new = replace_var(tmp, env, *i, var_len);
	return (new);
}

static char	*process_dollar(char *tmp, t_expand *ex)
{
	char	*new;

	new = handle_dollar(tmp, ex->env, ex->last_status, &ex->i);
	if (!new)
		return (NULL);
	if (new != tmp)
	{
		ex->quote = 0;
		ex->i = 0;
	}
	return (new);
}

char	*expand_variables(char *input, t_env *env, int last_status)
{
	t_expand	ex;
	char		*tmp;

	ex.i = 0;
	ex.quote = 0;
	ex.env = env;
	ex.last_status = last_status;
	tmp = ft_strdup(input);
	if (!tmp)
		return (NULL);
	while (tmp[ex.i])
	{
		update_quote(tmp[ex.i], &ex.quote);
		if (tmp[ex.i] == '$' && ex.quote != '\'')
		{
			tmp = process_dollar(tmp, &ex);
			if (!tmp)
				return (NULL);
		}
		else
			ex.i++;
	}
	return (tmp);
}
