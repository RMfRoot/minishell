/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egeorgel <egeorgel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:58:18 by egeorgel          #+#    #+#             */
/*   Updated: 2023/06/02 17:21:37 by egeorgel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	change_oldpwd(t_data *data, char *pwd)
{
	t_env	*buf;
	t_env	*oldpwd;
	char	*join;

	oldpwd = get_in_env(data, "OLDPWD");
	if (!oldpwd)
	{
		oldpwd = almost_last_env(data);
		join = ft_strjoin("OLDPWD=", pwd);
		buf = oldpwd->next;
		oldpwd->next = create_node(join);
		oldpwd->next->next = buf;
		free(join);
	}
	else
	{
		free(oldpwd->full_string);
		oldpwd->full_string = ft_strjoin("OLDPWD=", pwd);
		free(oldpwd->string);
		oldpwd->string = ft_strdup(pwd);
		oldpwd->is_equal = true;
	}
}

static void	create_pwd(t_data *data, t_env *env, char *pwd)
{
	t_env	*buf;
	char	*str;

	str = ft_strjoin("PWD=", pwd);
	env = almost_last_env(data);
	buf = env->next;
	env->next = create_node(str);
	env->next->next = buf;
	free(str);
}

static void	change_pwd(t_data *data, char *dir)
{
	t_env	*env;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = get_pwd(data, dir);
	env = get_in_env(data, "PWD");
	if (!env)
		create_pwd(data, env, pwd);
	else
	{
		free(env->string);
		env->string = ft_strdup(pwd);
		free(env->full_string);
		env->full_string = ft_strjoin("PWD=", pwd);
		env->is_equal = true;
	}
	free(data->pwd);
	data->pwd = ft_strdup(pwd);
	free(pwd);
}

void	cd_fction(t_data *data, char *input)
{
	char	*pwd;
	t_env	*home;

	if (!input)
	{
		home = get_in_env(data, "HOME");
		if (!home)
		{
			error(ERR_HOME_CD, NULL, NULL, data);
			return ;
		}
		input = home->string;
	}
	pwd = get_str_env(data, "PWD");
	if (!pwd)
		pwd = ft_strdup("");
	if (chdir(input) != 0)
	{
		error(ERR_MAX, NULL, NULL, data);
		free(pwd);
		return ;
	}
	change_pwd(data, input);
	change_oldpwd(data, pwd);
	free(pwd);
}

void	mini_cd(t_data *data, char **input)
{
	if (input[1] && input[2])
	{
		data->cmd_status = 2;
		error(ERR_ARGS, input[0], NULL, data);
		return ;
	}
	else if (ft_strcmp(input[1], "-"))
		cd_oldpwd(data, input);
	else if (is_flaged(input))
	{
		data->cmd_status = 2;
		error(ERR_FLAG, input[0], input[1], data);
		return ;
	}
	else
		cd_fction(data, input[1]);
}
