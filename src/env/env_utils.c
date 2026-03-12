/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 10:37:10 by egonin            #+#    #+#             */
/*   Updated: 2026/03/12 11:47:21 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

void    env_set(t_env **env, char *key, char *value)
{
    t_env   *tmp;
    t_env   *new;

    if (!env || !key || !value)
        return ;
    tmp = *env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            return ;
        }
        tmp = tmp->next;
    }
    new = env_new(key, value);
    if (!new)
        return ;
    env_add_back(env, new);
}
