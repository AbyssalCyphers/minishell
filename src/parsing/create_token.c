#include "../../include/minishell.h"

static int length_cmd(char *command, int *quotes)
{
    int i = 0;
    char quote_char = 0;
    *quotes = 0;

    while (command[i] && !is_space(command[i]) && !is_special(command + i))
    {
        if ((command[i] == '"' || command[i] == '\'') && !quote_char)
        {
            quote_char = command[i];
            (*quotes)++;
            i++;
            while (command[i] && command[i] != quote_char)
                i++;
            if (command[i] == quote_char)
            {
                (*quotes)++;
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    return i;
}

static void copy_token(char *command, int length, char *str)
{
    int i = 0;
    while (i < length)
    {
        str[i] = command[i];
        i++;
    }
    str[i] = '\0';
}

static bool add_cmd(t_token **begin, char **command)
{
    char    *str;
    int     length;
    int     quotes;

    length = length_cmd(*command, &quotes);
    str = malloc(sizeof(char) * (length + 1));
    if (!str)
        return (false);
    copy_token(*command, length, str);
    if (!append_token(begin, str, 0))
    {
        free(str);
        return (false);
    }
    if ((*begin)->prev == (*begin) || (*begin)->prev->prev->type == PIPE)
        (*begin)->prev->type = CMD;
    else
        (*begin)->prev->type = ARG;
    
    *command += length;
    return (true);
}

static bool add_special(t_token **begin, char **command)
{
    int spe;

    spe = is_special(*command);
    if (!spe)
        return (false);
    if (spe == INPUT && !append_token(begin, ft_strdup("<"), INPUT))
        return (false);
    else if (spe == HEREDOC && !append_token(begin, ft_strdup("<<"), HEREDOC))
        return (false);
    else if (spe == TRUNC && !append_token(begin, ft_strdup(">"), TRUNC))
        return (false);
    else if (spe == APPEND && !append_token(begin, ft_strdup(">>"), APPEND))
        return (false);
    else if (spe == PIPE && !append_token(begin, ft_strdup("|"), PIPE))
        return (false);
    if (spe == INPUT || spe == TRUNC || spe == PIPE)
        (*command)++;
    else if (spe == HEREDOC || spe == APPEND)
        (*command) += 2;
    return (true);
}

static bool add_heredoc_delimiter(t_token **begin, char **command)
{
    while (**command && is_space(**command))
        (*command)++;

    if (**command == '<' || **command == '>' || **command == '\0')
    {
        write(2, "syntax error near unexpected token `<<'\n", 40);
        return false;
    }

    char *start = *command;
    char quote = 0;
    int len = 0;

    // Count the length of the delimiter
    while (**command)
    {
        if (!quote && (**command == '\'' || **command == '"'))
        {
            quote = **command;
        }
        else if (quote && **command == quote)
        {
            quote = 0;
        }
        else if (!quote && (is_space(**command) || is_special(*command)))
        {
            break;
        }
        (*command)++;
        len++;
    }

    // Check if the delimiter is empty
    if (len == 0)
    {
        write(2, "syntax error near unexpected token `newline'\n", 45);
        return false;
    }

    char *delimiter = ft_substr(start, 0, len);
    if (!delimiter || !append_token(begin, delimiter, ARG))
    {
        free(delimiter);
        return false;
    }

    return true;
}

bool create_list_token(t_token **begin, char *command)
{
    (*begin) = NULL;
    while (*command)
    {
        while (is_space(*command))
            command++;
        if (*command && is_special(command))
        {
            if (!add_special(begin, &command))
            {
                if (*begin)
                    free_token(begin);
                return false;
            }
            // If we just added a HEREDOC token, add the delimiter as a single token
            if ((*begin)->prev->type == HEREDOC)
            {
                if (!add_heredoc_delimiter(begin, &command))
                {
                    free_token(begin);
                    return false;
                }
            }
        }
        else if (*command && !add_cmd(begin, &command))
        {
            if (*begin)
                free_token(begin);
            return false;
        }
    }
    return true;
}
