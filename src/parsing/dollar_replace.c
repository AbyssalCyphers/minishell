/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_replace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravenclaw <ravenclaw@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:03:52 by ravenclaw         #+#    #+#             */
/*   Updated: 2024/09/14 23:10:12 by ravenclaw        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	in_env(t_data *data, char *line, int size, char **str)
{
	char	*tmp;
	char	*key;
	char	*value;

	key = get_dollar_word(line, size);
	value = get_elem_env(data->env, key);
	if (key)
		free(key);
	tmp = ft_strjoin(*str, value);
	if (value)
		free(value);
	free(*str);
	if (!tmp)
		return (0);
	*str = tmp;
	return (1);
}

int	dollar_point_interrogation(t_data *data, char **str)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_itoa(data->exit_code);
	if (!tmp)
		return (0);
	tmp2 = ft_strjoin(*str, tmp);
	free(tmp);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}

int	add_dollar(char *line, int *index, char **str, t_data *data)
{
	int		ctrl;
	int		n;

	n = *index;
	ctrl = exist_in_env(line, index, data);
	if (ctrl == 1)
		return (in_env(data, &line[n], *index - n, str));
	else if (ctrl == 2)
	{
		(*index) += 2;
		return (dollar_point_interrogation(data, str));
	}
	else
	{
		++(*index);
		while (line[*index] && \
			(ft_isalnum(line[*index]) || line[*index] == '_'))
			++(*index);
		return (1);
	}
}

int	add_char(char *c, char **str, int *index)
{
	char	char_to_str[2];
	char	*tmp2;

	char_to_str[0] = *c;
	char_to_str[1] = '\0';
	(*index)++;
	tmp2 = ft_strjoin(*str, char_to_str);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}

int	process_heredoc(char **line, int *i, char **str)
{
	int in_quotes = 0;
	char quote_char = 0;

	// Add '<<' to the string
	if (!add_char(&(*line)[*i], str, i) || !add_char(&(*line)[*i], str, i))
		return (0);
	
	// Skip whitespace
	while ((*line)[*i] && ((*line)[*i] == ' ' || (*line)[*i] == '\t'))
		if (!add_char(&(*line)[*i], str, i))
			return (0);
	
	// Process heredoc delimiter
	while ((*line)[*i] && (in_quotes || ((*line)[*i] != ' ' && (*line)[*i] != '\t' && (*line)[*i] != '\n')))
	{
		if ((*line)[*i] == '"' || (*line)[*i] == '\'')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = (*line)[*i];
			}
			else if ((*line)[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
		}
		if (!add_char(&(*line)[*i], str, i))
			return (0);
	}
	
	return (1);
}

int replace_dollar(char **line, t_data *data)
{
    bool dq;
    int i;
    char *str;

    i = 0;
    dq = false;
    data->sq = false;
    str = ft_strdup("");
    if (!str)
        return (0);

    while ((*line)[i])
    {
		if ((*line)[i] == '<' && (*line)[i + 1] == '<')
		{
			if (!process_heredoc(line, &i, &str))
			{
				free(str);
				return (0);
			}
			continue;
		}
        quoting_choice(&dq, &data->sq, NULL, (*line)[i]);
        if ((*line)[i] && (*line)[i + 1] && (*line)[i] == '$' && 
            ((*line)[i + 1] != '\'' && (*line)[i + 1] != '"') && 
            (ft_isalpha((*line)[i + 1]) || (*line)[i + 1] == '?' || 
            (*line)[i + 1] == '_') && !data->sq)
        {
            char var_name[256] = {0};
            int var_i = 0;
            i++; // Skip the $
            if ((*line)[i] == '?')
            {
                char *tmp = ft_itoa(data->exit_code);
                char *new_str = ft_strjoin(str, tmp);
                free(str);
                free(tmp);
                if (!new_str)
                    return (0);
                str = new_str;
                i++;
            }
            else
            {
                while ((*line)[i] && (ft_isalnum((*line)[i]) || (*line)[i] == '_'))
                    var_name[var_i++] = (*line)[i++];
                var_name[var_i] = '\0';
                char *value = get_elem_env(data->env, var_name);
                if (value)
                {
                    // Remove surrounding quotes from the value
                    char *unquoted_value = value;
                    int len = ft_strlen(value);
                    if (len >= 2 && ((value[0] == '"' && value[len-1] == '"') || 
                                     (value[0] == '\'' && value[len-1] == '\'')))
                    {
                        unquoted_value = ft_substr(value, 1, len - 2);
                        if (!unquoted_value)
                            return (0);
                    }
                    char *new_str = ft_strjoin(str, unquoted_value);
                    free(str);
                    if (unquoted_value != value)
                        free(unquoted_value);
                    if (!new_str)
                        return (0);
                    str = new_str;
                }
            }
        }
        else if ((*line)[i])
        {
            char *new_str = ft_strjoin_char(str, (*line)[i]);
            free(str);
            if (!new_str)
                return (0);
            str = new_str;
            i++;
        }
    }
    free(*line);
    *line = str;
    return (1);
}

char *ft_strjoin_char(char const *s1, char c)
{
    size_t len = ft_strlen(s1);
    char *result = malloc(len + 2);
    if (!result)
        return NULL;
    
    strcpy(result, s1);
    result[len] = c;
    result[len + 1] = '\0';
    return result;
}
