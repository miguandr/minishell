/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:19:08 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/26 22:52:02 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles various types of errors by printing appropriate error messages.
 * @data: Pointer to the minishell structure (unused in this function).
 * @error: Error code indicating the type of error.
 *
 * This function prints an error message based on the error code. It sets
 * the exit code in the minishell structure according to the type of error
 * and resets the minishell data if necessary.
 */
int	handle_error(t_mshell *data, int error)
{
	const char	*message;
	const char	*error_message[] = {
		"memory error: unable to allocate memory\n",
		"syntax error: unable to find closing quotation\n",
		"syntax error: command line can not begin/end with a token\n",
		"syntax error: too many consecutive tokens\n",
		"syntax error: wrong token handling\n",
		"system error: failed to create child process\n",
		"syntax error: not a valid builtin\n",
		"system error: failed to create pipe\n",
		"system error: error in fd\n",
	};

	message = error_message[error];
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(message, 2);
	if (error == 8)
		data->exit_code = 1;
	else
		data->exit_code = 2;
	reset_data(data);
	return (EXIT_FAILURE);
}

/**
 * Handles specific error cases with additional information.
 * @data: Pointer to the minishell structure (unused in this function).
 * @error: Error code indicating the type of error.
 * @str: Additional information to include in the error message.
 * @array: Array of strings for error details.
 *
 * This function prints specific error messages based on the error code. It
 * provides additional context depending on the error, such as invalid
 * identifiers or event not found errors.
 */
int	handle_error2(t_mshell *data, int error, char *str, char **array)
{
	(void)data;
	if (error == 1 || error == 2 || error == 4)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		if (error == 1)
			print_array(array, 2);
		else if (error == 2 || error == 4)
			ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	}
	else if (error == 3)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(": event not found", STDERR_FILENO);
	}
	else if (error == 5)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	return (EXIT_FAILURE);
}
