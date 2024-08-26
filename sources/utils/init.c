/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:10:52 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/26 18:54:00 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Initializes the t_mshell structure with default values.
 * @data: Pointer to the t_mshell structure to be initialized.
 *
 * This function sets the initial values for the t_mshell structure fields,
 * including setting pointers to NULL, initializing boolean flags, and
 * configuring the environment and signal handlers. It returns 1 on success.
 */
int	init_data(t_mshell *data)
{
	data->commands = NULL;
	data->lexer_list = NULL;
	data->reset = false;
	data->pid = NULL;
	data->pipes = 0;
	handle_envp(data);
	init_signals();
	return (1);
}

/**
 * Resets the t_mshell structure, freeing allocated memory and reinitializing.
 * @data: Pointer to the t_mshell structure to be reset.
 *
 * This function clears the current command list, frees dynamically allocated
 * memory, reinitializes the t_mshell structure using init_data, and sets the
 * reset flag to true. It then calls minishell to restart the shell. Returns 1.
 */
int	reset_data(t_mshell *data)
{
	ft_commands_clear(&data->commands);
	free(data->args);
	if (data->pid)
		free(data->pid);
	ft_free_array(data->paths);
	init_data(data);
	data->reset = true;
	minishell(data);
	return (1);
}
