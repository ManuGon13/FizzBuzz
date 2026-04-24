/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:22:09 by egonin            #+#    #+#             */
/*   Updated: 2026/04/24 17:31:31 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
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

/* représente les éléments du lexer */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* représente une commande exécutable */
typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	char			*heredoc_delim;
	int				heredoc_fd;
	int				heredoc_expand;
	int				append;
	t_token			*tokens;
	struct s_cmd	*next;
	char			**infiles;
	char			**outfiles;
	int				*out_append;
	int				infile_count;
	int				outfile_count;
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
	int		should_exit;
}	t_shell;

/* s'occupe de l'executor */
typedef struct s_exec
{
	t_env	*env;
	int		*fd;
	int		prev_fd;
}	t_exec;

/* s'occupe des expansions */
typedef struct s_expand
{
	t_env	*env;
	int		last_status;
	int		i;
	char	quote;
}	t_expand;

/* Prototypes heredoc */
int		has_quotes(char *str);
void	heredoc_sigint(int sig);
int		prepare_cmd_heredocs(t_cmd *cmd, t_shell *shell);
int		handle_heredoc(t_cmd *cmd, t_shell *shell);
int		prepare_heredocs(t_cmd *cmd, t_shell *shell);
void	handle_signal(int sig);

/* Prototypes expansion */
int		get_var_len(char *input, int i);
int		is_var_char(char c);
void	update_quote(char c, char *quote);
char	*expand_variables(char *input, t_env *env, int last_status);

/* Prototypes clear and free*/
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);
void	free_env(t_env *env);
void	close_heredoc_fd(int *fd);

/* Prototypes env */
t_env	*env_new(char *key, char *value);
t_env	*env_init(char **envp);
void	env_add_back(t_env **env, t_env *new);
char	*get_env_value(t_env *env, const char *key);
void	env_set(t_env **env, const char *key, const char *value);
void	env_delone(t_env *node);
void	env_unset(t_env **env, char *key);
int		env_size(t_env *env);
void	print_tokens(t_token *tokens);
char	**env_to_array(t_env *env);
int		ft_strcmp(const char *s1, const char *s2);

/* Prototypes des builtins*/
char	*extract_value(char *arg);
int		ft_echo(char **argv);
int		ft_unset(t_env **env, char **argv);
int		ft_exit(char **argv);
int		ft_env(t_env *env);
int		ft_cd(t_cmd *cmd, t_env *env);
int		ft_pwd(t_env *env);
void	print_export(t_env *env);
int		ft_export(t_cmd *cmd, t_env **env);

/* Prototype du lexer */
int		check_quotes(char *input, int *i);
int		has_unclosed_quote(char *input);
void	token_add_back(t_token **list, t_token *new);
t_token	*token_new(char *value, t_token_type type);
t_token	*lexer(char *input);

/* Prototype parser */
void	if_heredoc(t_token **tokens, t_cmd *cmd);
int		add_arg(t_cmd *cmd, char *arg);
t_cmd	*new_cmd(void);
int		token_word(char *value, t_cmd *cmd, t_shell *shell);
int		token_pipe(t_cmd **cmd);
char	*remove_quotes(char *str);
t_cmd	*parser(t_token *tokens, t_shell *shell);
int		init_redir_arrays(t_cmd *cmd, t_token *tokens);
int		validate_pipe_syntax(t_token *tokens, t_shell *shell);

/* Prototypes executor */
void	close_child_stdio(void);
int		has_redirections(t_cmd *cmd);
void	close_next_heredocs(t_cmd *cmd);
void	close_future_heredoc_fds(t_cmd *cmd);
void	apply_redir_side_effects(t_exec *exec, t_cmd *cmd, char **envp);
int		last_input_is_heredoc(t_token *tokens);
void	handle_exec_signal(int sig);
int		get_exit_status(int status);
int		ft_wait(pid_t last_pid);
void	process_exec(t_exec *exec, t_cmd *cmd);
void	dup2_or_fail(int oldfd, int newfd, char **envp, int *fds_to_close);
void	process_outfile_next(t_exec *exec, t_cmd *cmd, char **envp);
void	finish_execution(t_exec *exec, t_cmd *cmd, char **envp);
void	prepare_exec(t_cmd *cmd, char **envp);
int		apply_stdin_redirection(t_cmd *cmd);
int		exec_builtin_parent(t_exec *exec, t_cmd *cmd);
int		exec_builtin(t_cmd *cmd, t_exec *exec);
int		is_builtin(t_cmd *cmd);
int		builtin_must_run_in_parent(t_cmd *cmd);
void	struct_exec_init(t_exec *exec, t_env *env, int *fd, int prev_fd);
void	closing(t_cmd *cmd, int *prev_fd, int *fd);
char	**find_path(char **envp);
void	execution(t_cmd *cmd, t_shell *shell);
char	*good_path(char *command, char **paths);
void	free_split(char **split);
void	print_error(char *command, int i);
void	infile_fail(t_cmd *cmd, int *fd, int prev_fd, char **envp);
void	outfile_fail(t_cmd *cmd, int *fd, int prev_fd, char **envp);

#endif