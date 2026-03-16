/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egonin <egonin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:05:52 by egonin            #+#    #+#             */
/*   Updated: 2026/03/16 11:05:48 by egonin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include "libft.h"

/* représente les éléments du lexer */
typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

/* représente une commande exécutable */
typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
}	t_cmd;

/* représente les variables d'environnement */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* stocke l'état global du shell */
typedef struct s_shell
{
	t_env	*env;
	int		exit_status;
}	t_shell;

/* Prototypes env */
t_env	*env_new(char *key, char *value);
t_env	*env_init(char **envp);
void	env_add_back(t_env **env, t_env *new);
char	*get_env_value(t_env *env, const char *key);
void	env_set(t_env **env, char *key, char *value);
void	env_delone(t_env *node);
void	env_unset(t_env **env, char *key);
int		env_size(t_env *env);

/* Définie les types de tokens dans tout le proejt */
enum	e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
} t_token_type;

#endif