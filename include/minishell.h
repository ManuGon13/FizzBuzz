/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltourbe <ltourbe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:22:09 by egonin            #+#    #+#             */
/*   Updated: 2026/03/12 10:50:52 by ltourbe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include	<pthread.h>
# include	<sys/time.h>
# include	<stdlib.h>
# include	<unistd.h>
# include	<stdio.h>
# include	<limits.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	**argv;
	char	*infile;
	char	*outfile;
	int		append;
	
	struct s_cmd	*next;

} t_cmd;

typedef struct	s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
} t_env;

typedef struct s_shell
{
	t_env	*env;
	int	exit_status;
} t_shell;

enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
};

#endif