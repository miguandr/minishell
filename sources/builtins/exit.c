/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:58:10 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:58:10 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_minishell(t_mshell *minishell)
{
	free_lexer_list(minishell->commands->redirections);
	free_parser_list(minishell->commands);
	free_lexer_list(minishell->lexer_list);
	free_string_array(minishell->paths);
	free_string_array(minishell->envp);
	if (minishell->pwd)
		free(minishell->pwd);
	if (minishell->old_pwd)
		free(minishell->old_pwd);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell->args)
		free(minishell->args);
}

static int	is_num(char *num)
{
	int	i;

	i = 0;
	if (num[i] == '+' || num[i] == '-')
		i++;
	while (num[i])
	{
		if (!ft_isdigit(num[i]))
			return (0);
		else
			i++;
	}
	return (1);
}

/*
 * This function processes the exit command by checking the provided arguments.
 * If no additional argument is given, the exit code is set to 0. If a numeric 
 * argument is provided, it sets the exit code accordingly. If there are too 
 * many arguments or a non-numeric argument is passed, it handles the error, 
 * sets the appropriate exit code, and returns failure.
 * Finally, it frees the allocated resources and exits the program with the 
 * specified exit code.
 * Returns EXIT_FAILURE if there are too many arguments or an error occurs.
 */
int	mini_exit(t_mshell *data, t_parser *commands)
{
	if (commands == NULL || data == NULL)
		return (EXIT_FAILURE);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!commands->str[1])
		data->exit_code = 0;
	else if (is_num(commands->str[1]) && commands->str[2])
	{
		data->exit_code = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else
	{
		if (!is_num(commands->str[1]))
			data->exit_code = handle_error2(data, 5, commands->str[1], NULL);
		else
			data->exit_code = ft_atoi(commands->str[1]);
	}
	free_minishell (data);
	exit (data->exit_code);
}
