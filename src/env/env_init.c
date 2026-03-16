/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:05:33 by egonin            #+#    #+#             */
/*   Updated: 2026/03/16 15:16:26 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

t_env	*env_init(char **envp)
{
	int		i;
	t_env	*env;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '=') != NULL)
		{
			equal = ft_strchr(envp[i], '=');
			key = ft_substr(envp[i], 0, equal - envp[i]);
			value = ft_strdup(equal + 1);
			env_add_back(&env, env_new(key, value));
		}
		i++;
	}
	return (env);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
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
	t_token	t1;
	t_token	t2;
	t_token	t3;

	t1.value = "ls      ";
	t1.next = &t2;

	t2.value = ">>";
	t2.next = &t3;

	t3.value = "file.txt";
	t3.next = NULL;

	print_tokens(&t1);

	return (0);
}