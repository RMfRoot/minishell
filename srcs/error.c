/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egeorgel <egeorgel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 10:16:37 by egeorgel          #+#    #+#             */
/*   Updated: 2023/06/19 23:41:24 by egeorgel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*add_token(char *msg, char *token)
{
	msg = ft_strjoinfree(msg, " '", true, false);
	msg = ft_strjoinfree(msg, token, true, false);
	msg = ft_strjoinfree(msg, "'", true, false);
	return (msg);
}

void	error_exit(int err, char *input, char *token, t_data *data)
{
	char	*msg;
	int		saved;

	if (input)
		input = ft_strjoinfree(": ", input, false, true);
	msg = ft_strjoinfree("minishell", input, false, true);
	if (errno && !err)
		perror(msg);
	else
	{
		saved = dup(1);
		msg = ft_strjoinfree(msg, ": ", true, false);
		msg = ft_strjoinfree(msg, data->errlst[err], true, false);
		if (token)
			msg = add_token(msg, token);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("%s\n", msg);
		dup2(saved, STDOUT_FILENO);
	}
	free(msg);
	if (data->cmd_status)
		exit(data->cmd_status);
	else
		exit(1);
}

void	error(int err, char *input, char *token, t_data *data)
{
	char	*msg;
	int		saved;

	if (input)
	{
		input = ft_strjoinfree(": ", input, false, false);
		msg = ft_strjoinfree("minishell", input, false, true);
	}
	else
		msg = ft_strjoinfree("minishell", input, false, false);
	if (errno)
		perror(msg);
	else
	{
		saved = dup(1);
		msg = ft_strjoinfree(msg, ": ", true, false);
		msg = ft_strjoinfree(msg, data->errlst[err], true, false);
		if (token)
			msg = add_token(msg, token);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("%s\n", msg);
		dup2(saved, STDOUT_FILENO);
	}
	free(msg);
	if (!data->cmd_status && !data->status)
		data->cmd_status = 1;
}
/*
written like: "minishell: input: err_msg 'token'"
example: "minishell: cat_: command not found"
example: "minishell: empty argument after token '<'"
example: "minishell: pwd: Following option is invalid '-l'"
*/

void	get_errlst(t_data *data)
{
	data->errlst = malloc(sizeof(char *) * (ERR_MAX + 1));
	data->errlst[ERR_CMD] = strdup("Command not found");
	data->errlst[ERR_DIR] = strdup("Is a directory");
	data->errlst[ERR_NOT_DIR] = strdup("Not a directory");
	data->errlst[ERR_FD] = strdup("No such file or directory");
	data->errlst[ERR_PARSING] = strdup("Unexpected parsing error");
	data->errlst[ERR_UNSUPORTED] = strdup("Is not a valid input");
	data->errlst[ERR_HOME_HIS]
		= strdup("Could not access history, $HOME is not set");
	data->errlst[ERR_HOME]
		= strdup("Could not get character '~', $HOME is not set");
	data->errlst[ERR_HOME_CD] = strdup("$HOME is not set");
	data->errlst[ERR_OLDPWD] = strdup("$OLDPWD is not set");
	data->errlst[ERR_START_PWD]
		= strdup("Can't start minishell in a non existing directory");
	data->errlst[ERR_QUOTES] = strdup("Unclosed quotes");
	data->errlst[ERR_EXIT] = strdup("numeric argument required");
	data->errlst[ERR_EMPTY] = strdup("syntax error near token");
	data->errlst[ERR_FLAG] = strdup("Following option is invalid");
	data->errlst[ERR_ARGS] = strdup("Invalid command arguments");
	data->errlst[ERR_ARGS_NBR] = strdup("Too many arguments");
	data->errlst[ERR_EXP] = strdup("not a valid indentifier");
	data->errlst[ERR_PERM] = strdup("Permission denied");
	data->errlst[ERR_REDIR] = strdup("ambiguous redirect");
	data->errlst[ERR_MAX] = NULL;
}
