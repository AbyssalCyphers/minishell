#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../Libft/src/libft.h"
//# include "../Libft/src/get_next_line.h"

# define ERR_MALLOC	"malloc error\n"
# define ERR_PIPE	"pipe error\n"
# define ERR_FORK	"fork error\n"
# define PATH_MAX	4096

# define EXT_MALLOC	1
# define EXT_PIPE	1
# define EXT_FORK	1

# define INPUT		1	//"<"
# define HEREDOC	2	//"<<"
# define TRUNC		3	//">"
# define APPEND		4	//">>"
# define PIPE		5	//"|"
# define CMD		6	//"command"
# define ARG		7	//"argument"

extern pid_t	g_spid;

typedef struct s_cmd
{
	bool			skip_cmd;
	int				infile;
	int				outfile;
	char			**cmd_param;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct s_list
{
	char			*str;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

typedef struct s_data
{
	t_list	*env;
	t_token	*token;
	t_cmd	*cmd;
	int		exit_code;
	int 	pipe[2] ;
	bool	sq;
}				t_data;

/* main */
int		make_env(t_data *data, char **env);

/* List utils */
int		free_list(t_list **list);
int		append(t_list **list, char *elem);
size_t	len_list(t_list *list);

/* quote */
void	quoting_choice(bool *dq, bool *sq, int *index, char c);
int		open_quote(t_data *data, char *line);

/* dollar_env */
int		exist_in_env(char *line, int *i, t_data *data);
char	*get_elem_env(t_list *env, char *key);
char	*get_dollar_word(char *line, int size);

/* dollar_replace */
int		add_dollar(char *line, int *index, char **str, t_data *data);
int		replace_dollar(char **line, t_data *data);
char *ft_strjoin_char(char const *s1, char c);

//create_token.c
bool	create_list_token(t_token **begin, char *command);

//list_token.c
int		append_token(t_token **list, char *str, int type);
void	free_token(t_token **list);

//ms_utils.c
bool	is_space(char c);
int		is_special(char *str);
bool	check_pipe(t_data *data);
char *trim_quotes(const char *str);

//free.c
void	free_array(char **arr);
bool	print_error(char *str);
void	free_all(t_data *data, char *err, int ext);
bool	print_error_token(t_token *token, t_data *data);

//list_cmd.c
int		append_cmd(t_cmd **list, int infile, int outfile, char **cmd_param);
void	free_cmd(t_cmd **list);
size_t	len_cmd(t_cmd *list);

//create_cmd.c
bool	create_list_cmd(t_data *data);

//cmd_fd.c
bool	get_infile(t_data *data, t_token *token, t_cmd *cmd);
bool	get_outfile(t_token *token, t_cmd *cmd, t_data *data);

//here_doc.c
int		here_doc(t_data *data, char *word);

//cmd_param.c
char	**get_param(t_data *data, t_token *token);

// ********        DEBUG        ********** //
void	print_token(t_token *token);
void 	print_command_structure(t_cmd *cmd);
void 	print_tab(char **tab);

//signalStuff
void clear_rl_line(void);
void signals(void);

//envStuff
int 	ft_env(t_list *env);
//builtings
int 	ft_export(char **str, t_list **env);
bool 	export_error(char *str, t_list **env);
int		ft_echo(char **args);
int		ft_unset(char **str, t_list **env);
int		ft_pwd(void);
int		ft_cd(t_data *data, char **params);
void	ft_exit(t_data *data, char **args);
//someUtils
char	**lst_to_arr(t_list *env);
void	sort_array(char **arr, int len);
//fdStuff
bool	get_infile(t_data *data, t_token *token, t_cmd *cmd);
bool	get_outfile(t_token *token, t_cmd *cmd, t_data *data);

//exec.c
bool	exec(t_data *data);
bool	is_builtin(char *cmd);

//launch_builtin.c
bool	launch_builtin(t_data *data, t_cmd *cmd);

//find_cmd.c
char	*find_cmd(t_data *data, char *sample, t_list *env);

//exec2.c
void	child_process(t_data *data, t_cmd *cmd, int *pip);

//signals2.c
void	signals2(void);

void    abosulte_path(char **path, char *cmd, t_data *data);

#endif