#include "../include/minishell.h"

int	make_env(t_data *data, char **env)
{
	t_list	*list;
	int		i;
	char	*tmp;

	if (!(*env))
		return (0);
	i = -1;
	list = NULL;
	while (env[++i])
	{
		tmp = ft_strdup(env[i]);
		// printf("env :%s\n", tmp);
		if (!tmp)
			return (free_list(&list));
		if (!append(&list, tmp))
			return (free_list(&list));
	}
	data->env = list;
	return (1);
}

void	init_data(t_data *data, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	data->env = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->exit_code = 0;
}

bool	empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	if (i == (int)ft_strlen(line))
	{
		free(line);
		return (true);
	}
	return (false);
}

bool	parser(t_data *data, char *line)
{
	if (open_quote(data, line))
	{
		free(line);
		return (false);
	}
	if (!replace_dollar(&line, data))
    {
        free(line);
        free_all(data, ERR_MALLOC, EXT_MALLOC);
    }

	if (!create_list_token(&data->token, line))
	{
		free(line);
		free_token(&data->token);
		data->exit_code = 2;
		return false;
	}
	free(line);
	if (!check_pipe(data)) //segmentation fault to fix
    {
        return (false);
    }
	// check before calling print_token
	if (data->token == NULL)
	{
		printf("Warning: No tokens created\n");
		return (false);
	}
	print_token(data->token);
	if (data->token && data->token->prev->type == PIPE)
	{
		write(2, "Error: Unclosed pipe\n", 21);
		data->exit_code = 2;
		free_token(&data->token);
		return (false);
	}
	if (!data->token || !create_list_cmd(data))
	{
		free_token(&data->token);
		free_cmd(&data->cmd);
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*line;

	init_data(&data, argc, argv);
	if (!make_env(&data, env))
		free_all(&data, ERR_MALLOC, EXT_MALLOC);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			free_all(&data, "exit\n", data.exit_code);
		if (empty_line(line))
			continue ;
		add_history(line);
		if (!parser(&data, line))
			continue ;
		if (!exec(&data))
			free_all(&data, ERR_PIPE, EXT_PIPE);
		//print_command_structure(data.cmd);
		free_cmd(&data.cmd);
		free_token(&data.token);
	}
	rl_clear_history();
	free_all(&data, NULL, -1);
	return (0);
}


