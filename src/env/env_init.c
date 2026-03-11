/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:05:33 by egonin            #+#    #+#             */
/*   Updated: 2026/03/11 18:33:58 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_env    *env_init(char **envp)
{
    int     i;
    char    *equal;

    i = 0;
    while (envp[i])
    {
        if (ft_strchr(envp[i], '=') != NULL)
        {
            equal = ft_strchr(envp[i], '=');
            key = ft_substr(envp[i], 0, equal - envp[i]);
            value = ft_strdup(equal + 1);
        }
    }
        i++;
}
t_env    *env_init(char **envp)
{
    int     i;
    char    *equal;

    i = 0;
    while (envp[i])
    {
        if (ft_strchr(envp[i], '=') != NULL)
        {
            equal = ft_strchr(envp[i], '=');
            key = ft_substr(envp[i], 0, equal - envp[i]);
            value = ft_