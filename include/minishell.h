/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:22:09 by egonin            #+#    #+#             */
/*   Updated: 2026/03/19 16:39:46 by ltourbe          ###   ########.fr       */
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
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>

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
void	print_tokens(t_token *tokens);
char	**env_to_array(t_env *env);

/* Prototype du lexer */
t_token	*lexer(char *input);

/* Prototypes clear and free*/
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);
void	free_env(t_env *env);

/* Prototype parser */
t_cmd	*parser(t_token *tokens);

/* Prototypes executor */
void	execution(t_cmd *cmd, t_env *env);
void	prepare_exec(t_cmd *cmd, char **envp);
void	free_split(char **split);
void	print_error(char *command, int i);
void	infile_fail(t_cmd *cmd, int *fd, int prev_fd, char **envp);
void	outfile_fail(t_cmd *cmd, int *fd, int prev_fd, char **envp);

/* Définie les types de tokens dans tout le projet */
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_token_type;

#endif