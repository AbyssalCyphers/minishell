
#include "../../include/minishell.h"

static char *strip_quotes(char *str)
{
    int len = ft_strlen(str);
    if (len >= 2 && ((str[0] == '"' && str[len-1] == '"') || (str[0] == '\'' && str[len-1] == '\'')))
    {
        str[len-1] = '\0';
        return str + 1;
    }
    return str;
}

static int open_file(t_data *data, char *filename, int type)
{
    int fd;
    char *stripped_filename;

    stripped_filename = strip_quotes(filename);
    if (type != HEREDOC && (!stripped_filename || stripped_filename[0] == '\0'))
    {
        write(2, "minishell: : No such file or directory\n", 39);
        data->exit_code = 1;
        return -1;
    }
    if ((!filename || filename[0] == '\0') || (type != HEREDOC && (!stripped_filename || stripped_filename[0] == '\0')))
    {
        write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
        data->exit_code = 2;
        return -1;
    }

    fd = -2;
    if (type == INPUT)
        fd = open(stripped_filename, O_RDONLY, 0644);
    else if (type == HEREDOC)
        fd = here_doc(data, filename);
    else if (type == TRUNC)
        fd = open(stripped_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (type == APPEND)
        fd = open(stripped_filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
    
    if (type != HEREDOC && fd < 0)
        perror(stripped_filename);
    
    return fd;
}

static bool get_in(t_data *data, t_token *tmp, t_cmd *cmd)
{
    if (tmp->type == INPUT || tmp->type == HEREDOC)
    {
        if (cmd->infile >= 0)
            close(cmd->infile);
        if (tmp == tmp->next || tmp->next->type <= 5 || tmp->next == data->token)
            return (print_error_token(tmp, data));
        cmd->infile = open_file(data, tmp->next->str, tmp->type);
        if (cmd->infile == -1)
        {
            cmd->skip_cmd = true;
            return (true);
        }
    }
    return (true);
}
bool	get_infile(t_data *data, t_token *token, t_cmd *cmd)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->type != PIPE && !get_in(data, tmp, cmd))
		return (false);
	if (tmp->type == PIPE)
		return (true);
	tmp = tmp->next;
	while (tmp->type != PIPE && tmp != data->token)
	{
		if (!get_in(data, tmp, cmd))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

static bool get_out(t_token *tmp, t_cmd *cmd, t_data *data)
{
    if (tmp->type == TRUNC || tmp->type == APPEND)
    {
        if (cmd->outfile >= 0)
            close(cmd->outfile);
        if (tmp == tmp->next || tmp->next->type <= 5 || tmp->next == data->token)
            return (print_error_token(tmp, data));
        cmd->outfile = open_file(data, tmp->next->str, tmp->type);
        if (cmd->outfile == -1)
        {
            cmd->skip_cmd = true;
            return (true);
        }
    }
    return (true);
}

bool	get_outfile(t_token *token, t_cmd *cmd, t_data *data)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->type != PIPE && !get_out(tmp, cmd, data))
		return (false);
	tmp = tmp->next;
	while (tmp != data->token && tmp->type != PIPE)
	{
		if (!get_out(tmp, cmd, data))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}
