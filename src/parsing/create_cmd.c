
#include "../../include/minishell.h"

static bool	fill_cmd(t_data *data, t_token *tmp)
{
	if (!get_infile(data, tmp, data->cmd->prev) && \
		data->cmd->prev->infile != -1)
		return (false);
	if (data->cmd->prev->infile == -1)
	{
		data->cmd->prev->skip_cmd = true;
		data->cmd->prev->outfile = -1;
		return (true);
	}
	if (!get_outfile(tmp, data->cmd->prev, data) && data->cmd->prev->outfile \
		!= -1)
		return (false);
	if (data->cmd->prev->outfile == -1)
	{
		if (data->cmd->prev->infile >= 0)
			close (data->cmd->prev->infile);
		data->cmd->prev->skip_cmd = true;
		data->cmd->prev->infile = -1;
		return (true);
	}
	data->cmd->prev->cmd_param = get_param(data, tmp);
	if (!data->cmd->prev->cmd_param)
		free_all(data, ERR_MALLOC, EXT_MALLOC);
	return (true);
}

static bool	norm(t_data *data, t_token *tmp)
{
	if (!append_cmd(&data->cmd, -2, -2, NULL))
		free_all(data, ERR_MALLOC, EXT_MALLOC);
	if (!fill_cmd(data, tmp))
	{
		data->exit_code = 2;
		return (false);
	}
	return (true);
}

void trim_quotes_in_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;

    t_cmd *current = cmd;
    char **params;
    char *trimmed;

    while (1)
    {
        if (current->cmd_param)
        {
            params = current->cmd_param;
            while (*params)
            {
                trimmed = trim_quotes(*params);
                if (trimmed)
                {
                    free(*params);
                    *params = trimmed;
                }
                params++;
            }
        }
        current = current->next;
        if (current == cmd)
            break;
    }
}

static bool is_redirection(int type)
{
    return (type == INPUT || type == HEREDOC || type == TRUNC || type == APPEND);
}

bool create_list_cmd(t_data *data)
{
    t_token *tmp;

    tmp = data->token;
	if (is_redirection(tmp->prev->type))
    {
        write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
        data->exit_code = 2;
        return false;
    }
    if (!norm(data, tmp))
        return (false);
    tmp = tmp->next;
    while (tmp != data->token)
    {
        if (tmp->prev->type == PIPE)
        {
            if (!norm(data, tmp))
                return (false);
        }
        tmp = tmp->next;
    }

    // to trim quotes from all commands
    trim_quotes_in_cmd(data->cmd);

    return (true);
}

/*
bool create_list_cmd(t_data *data)
{
    t_token *tmp;

    tmp = data->token;
    if (!norm(data, tmp))
        return (false);
    tmp = tmp->next;
    while (tmp != data->token)
    {
        if (tmp->prev->type == PIPE)
        {
            if (!norm(data, tmp))
                return (false);
        }
        tmp = tmp->next;
    }

    // to trim quotes from all commands
    trim_quotes_in_cmd(data->cmd);

    return (true);
}*/
