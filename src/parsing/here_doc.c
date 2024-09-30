
#include "../../include/minishell.h"

static char *get_env_value(t_data *data, char *key)
{
    char *value = get_elem_env(data->env, key);
    return value ? value : "";
}

static int expand_variable(char **result, char *src, int *i, t_data *data)
{
    char var_name[256] = {0};
    int var_i = 0;
    char *tmp;

    (*i)++; // Skip the $
    if (src[*i] == '?')
    {
        tmp = ft_itoa(data->exit_code);
        *result = ft_strjoin(*result, tmp);
        free(tmp);
        (*i)++;
        return (1);
    }
    while (src[*i] && (ft_isalnum(src[*i]) || src[*i] == '_'))
    {
        var_name[var_i++] = src[(*i)++];
    }
    var_name[var_i] = '\0';
    char *value = get_env_value(data, var_name);
    tmp = ft_strjoin(*result, value);
    free(*result);
    *result = tmp;
    return (1);
}

int expand_heredoc(char **line, t_data *data)
{
    char *result = ft_strdup("");
    int i = 0;
    char *tmp;

    while ((*line)[i])
    {
        if ((*line)[i] == '$')
        {
            if (!expand_variable(&result, *line, &i, data))
            {
                free(result);
                return (0);
            }
        }
        else
        {
            tmp = ft_strjoin(result, (char[]){(*line)[i], '\0'});
            free(result);
            result = tmp;
            i++;
        }
    }

    free(*line);
    *line = result;
    return (1);
}

static bool is_delimiter_quoted(const char *word)
{
    int len = ft_strlen(word);
    return (len > 0 && (word[0] == '"' || word[0] == '\'' ||
                        word[len - 1] == '"' || word[len - 1] == '\''));
}

char *trim_quotes(const char *str)
{
    if (!str)
        return NULL;

    size_t len = ft_strlen(str);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;

    size_t i = 0;
    size_t j = 0;
    char quote = 0;

    while (str[i])
    {
        if (!quote && (str[i] == '\'' || str[i] == '"'))
        {
            quote = str[i];
        }
        else if (quote && str[i] == quote)
        {
            quote = 0;
        }
        else
        {
            result[j++] = str[i];
        }
        i++;
    }

    result[j] = '\0';

    if (j == 0)
    {
        free(result);
        return ft_strdup("");
    }

    char *final = ft_strdup(result);
    free(result);
    return final;
}

static bool read_in_stdin(t_data *data, int fd, char *word, bool quoted)
{
    char *buf;
    char *trimmed_word = trim_quotes(word);

    printf("is quoted : %d\n", quoted);
    while (1)
    {
        buf = readline("> ");
        if (!buf)
        {
            print_error("warning: here-document delimited by end-of-file ");
            print_error("(wanted '");
            print_error(trimmed_word);
            print_error("')\n");
            break;
        }
        if (!ft_strncmp(trimmed_word, buf, INT_MAX))
        {
            free(buf);
            break;
        }
        if (!quoted)
        {
            if (!expand_heredoc(&buf, data))
            {
                free(buf);
                return false;
            }
        }
        write(fd, buf, ft_strlen(buf));
        write(fd, "\n", 1);
        free(buf);
    }
    return true;
}

int here_doc(t_data *data, char *word)
{
    int fd;
    bool quoted = is_delimiter_quoted(word);

    fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
        return (-1);
    if (!read_in_stdin(data, fd, word, quoted))
    {
        close(fd);
        unlink(".heredoc.tmp");
        return (-1);
    }
    close(fd);
    fd = open(".heredoc.tmp", O_RDONLY);
    if (fd > 0)
        unlink(".heredoc.tmp");
    return (fd);
}
