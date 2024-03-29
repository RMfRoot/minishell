/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egeorgel <egeorgel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 15:15:00 by vkuzmin           #+#    #+#             */
/*   Updated: 2023/06/19 23:35:09 by egeorgel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	swap(t_env *a, t_env *b)
{
	char	*buf;

	buf = a->full_string;
	a->full_string = b->full_string;
	b->full_string = buf;
	buf = a->pref;
	a->pref = b->pref;
	b->pref = buf;
	buf = a->string;
	a->string = b->string;
	b->string = buf;
}

static int	strcmpp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	if (s1[i] && !s2[i])
		return (1);
	if (!s1[i] && s2[i])
		return (-1);
	return (0);
}

static void	clear_env(t_env *env)
{
	t_env	*buf;

	while (env)
	{
		buf = env->next;
		free(env->full_string);
		free(env->pref);
		free(env->string);
		free(env);
		env = buf;
	}
}

static void	print(t_data *data, t_env *buf)
{
	int	saved;

	saved = dup(1);
	dup2(data->out_fd, STDOUT_FILENO);
	while (buf)
	{
		if (*buf->string && (!ft_strcmp(buf->pref, "PATH") || data->print_path))
			printf("declare -x %s=\"%s\"\n", buf->pref, buf->string);
		else if ((!ft_strcmp(buf->pref, "PATH") || data->print_path))
			printf("declare -x %s\n", buf->full_string);
		buf = buf->next;
	}
	dup2(saved, STDOUT_FILENO);
}

void	sort_and_print(t_data *data, t_env *env)
{
	t_env	*next;
	t_env	*buf;

	buf = env;
	while (buf)
	{
		next = buf->next;
		while (next)
		{
			if (strcmpp(buf->pref, next->pref) > 0)
				swap(buf, next);
			next = next->next;
		}
		buf = buf->next;
	}
	buf = env;
	print(data, buf);
	clear_env(env);
}
