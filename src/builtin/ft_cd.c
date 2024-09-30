/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:38:32 by handler           #+#    #+#             */
/*   Updated: 2024/09/26 18:18:57 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	error_malloc(void)
{
    print_error(ERR_MALLOC);
    return ;
}

static t_list *create_node(char *str)
{
    t_list *new_node;

    new_node = (t_list *)malloc(sizeof(t_list));
    if (!new_node)
        return (NULL);
    new_node->str = strdup(str);
    if (!new_node->str)
    {
        free(new_node);
        return (NULL);
    }
    new_node->next = NULL;
    return (new_node);
}

static void expor_t(char *str, t_list **env)
{
    t_list *new_node;
    t_list *tmp;

    new_node = create_node(str);
    if (!new_node)
    {
        error_malloc();
        return;
    }
    if (!*env)
    {
        *env = new_node;
        return;
    }
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}

static int	count_arg(char **params)
{
	int	count;

	count = 0;
	while (params[count])
		count++;
	return (count);
}

static void	update_oldpwd(t_data *data)
{
	t_list	*tmp;
	char	*test;
	int		len;

	tmp = data->env;
	test = NULL;
	len = len_list(tmp);
	while (len--)
	{
		if (ft_strncmp(tmp->str, "PWD=", 3) == 0)
			test = tmp->str;
		tmp = tmp->next;
	}
	if (!test)
		expor_t("OLDPWD", &data->env);
	if (test)
	{
		test = ft_strjoin("OLD", test);
		if (!test)
			return (error_malloc());
		expor_t(test, &data->env);
	}
	free(test);
}

static void	update_pwd(t_data *data, char *param)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	update_oldpwd(data);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror(param);
		return ;
	}
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return (error_malloc());
	expor_t(pwd, &data->env);
	free(pwd);
}

int	ft_cd(t_data *data, char **params)
{
	int	res;

	if (count_arg(params) == 2)
	{
		res = chdir(params[1]);
		if (res == 0)
			update_pwd(data, params[1]);
		if (res == -1)
			res *= -1;
		if (res == 1)
			perror(params[1]);
		return (res);
	}
	return (1);
}
