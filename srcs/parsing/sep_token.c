/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egeorgel <egeorgel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 21:45:46 by egeorgel          #+#    #+#             */
/*   Updated: 2023/06/01 01:16:05 by egeorgel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	err_quotes(t_data *data, t_list *lst, bool *quotes)
{
	if (quotes[0] || quotes[1])
	{
		data->status = 1;
		error(ERR_QUOTES, NULL, NULL, data);
		save_history(data);
		ft_lstclear(&lst, free);
		return (false);
	}
	return (true);
}

void	sep_loop(char *str, int *j, bool *quotes)
{
	char	c;

	while (str[*j] && ft_strchr("|<>&(){}[];%*", str[*j])
		&& (!quotes[0] && !quotes[1]))
	{
		c = str[*j];
		if (str[*j] == '\'' && !quotes[1])
			quotes[0] = set_to_opposite(quotes[0]);
		else if (str[*j] == '"' && !quotes[0])
			quotes[1] = set_to_opposite(quotes[1]);
		(*j)++;
		if (str[*j] && str[*j] != c)
			break ;
	}
}

static int	get_seperator(char *str, t_data *data, t_list **buf, bool *quotes)
{
	int		i;
	int		j;

	i = 0;
	if (str[i] && ft_strchr(" \n\t\v\f\r", str[i]))
		i++;
	j = i;
	sep_loop(str, &j, quotes);
	if (i != j)
	{
		(*buf)->next = ft_lstnew(ft_substr(str, i, j - i));
		if (!check_sep_token((*buf)->next, data))
			return (-1);
	}
	if (str[j] && ft_strchr(" \n\t\v\f\r", str[j]))
		j++;
	return (j);
}

static int	get_token(char *str, t_data *data, t_list **buf, bool *quotes)
{
	int		i;
	int		j;

	i = get_seperator(str, data, buf, quotes);
	if (i == -1)
		return (INT_MIN);
	j = i;
	j = token_end(str, quotes, j);
	if ((*buf)->next)
	{
		if (i == j && (ft_strchr("<>", *(*buf)->next->str)
				|| (ft_strcmp("|", (*buf)->next->str)
					&& !ft_strchr("<>", str[j]))))
		{
			data->status = 258;
			error(ERR_EMPTY, NULL, (*buf)->next->str, data);
			return (INT_MIN);
		}
		else if (i == j)
			return (j);
		(*buf)->next->next = ft_lstnew(ft_substr(str, i, j - i));
	}
	else if (i != j)
		(*buf)->next = ft_lstnew(ft_substr(str, i, j - i));
	return (j);
}

t_list	*sep_token(char *str, t_data *data, bool *quotes)
{
	int		i;
	t_list	*lst;
	t_list	*buf;

	if (!str || !*str)
		return (NULL);
	ft_rem_double_space_quotes(str, " \n\t\v\f\r");
	lst = ft_lstnew(ft_strdup(""));
	buf = lst;
	i = 0;
	if (!check_first_pipe(data, str, quotes))
		return (NULL);
	while (str[i])
	{
		i += get_token(str + i, data, &buf, quotes);
		if (!invalid_token(data, lst, i))
			return (NULL);
		buf = ft_lstlast(buf);
	}
	if (!err_quotes(data, lst, quotes))
		return (NULL);
	buf = lst->next;
	ft_lstdelone(lst, free);
	return (buf);
}
